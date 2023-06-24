/*
Copyright 2017 Ioannis Makris

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation and/or
other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef SINTRA_UTILITY_H
#define SINTRA_UTILITY_H

#include <functional>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <string>
#include <unordered_set>
#include <chrono>


#ifdef _WIN32
    #include <process.h>
#else
    #include <cstring>
    #include <fcntl.h>
    #include <sys/wait.h>
    #include <unistd.h>
#endif


namespace sintra {


using std::function;
using std::shared_ptr;
using std::mutex;
using std::lock_guard;

struct Adaptive_function
{
    Adaptive_function(function<void()> f) :
        ppf(new shared_ptr<function<void()>>(new function<void()>(f))),
        m(new mutex)
    {}

    Adaptive_function(const Adaptive_function& rhs)
    {
        lock_guard<mutex> lock(*rhs.m);
        ppf = rhs.ppf;
        m = rhs.m;
    }

    void operator()()
    {
        lock_guard<mutex> lock(*m);
        (**ppf)();
    }

    void set(function<void()> f)
    {
        lock_guard<mutex> lock(*m);
        **ppf = f;
    }

    shared_ptr<shared_ptr<function<void()>>> ppf;
    shared_ptr<mutex> m;
};



inline 
size_t get_cache_line_size()
{
#ifdef _WIN32

    size_t line_size = 0;
    DWORD buffer_size = 0;
    DWORD i = 0;
    SYSTEM_LOGICAL_PROCESSOR_INFORMATION * buffer = 0;

    GetLogicalProcessorInformation(0, &buffer_size);
    buffer = (SYSTEM_LOGICAL_PROCESSOR_INFORMATION *)malloc(buffer_size);
    GetLogicalProcessorInformation(&buffer[0], &buffer_size);

    for (i = 0; i != buffer_size / sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION); ++i) {
        // assuming all cache levels have the same line size...
        if (buffer[i].Relationship == RelationCache && buffer[i].Cache.Level == 1) {
            line_size = buffer[i].Cache.LineSize;
            break;
        }
    }

    free(buffer);
    return line_size;

#else

    // TODO: implement
    return 0x40;

#endif
}



// C++ vector of strings to C style null terminated array of pointers
// conversion utility
struct cstring_vector
{
    cstring_vector(const std::vector<std::string>& v_in):
        m_size(v_in.size())
    {
        m_v = new const char*[m_size+1];
        for (size_t i = 0; i < m_size; i++) {
            m_v[i] = v_in[i].c_str();
        }
        m_v[m_size] = 0;
    }

    ~cstring_vector()
    {
        delete [] m_v;
    }

    const char* const* v() const { return m_v; }
    size_t size() const { return m_size; }

private:
    const char** m_v = nullptr;
    const size_t m_size = 0;
};



inline
bool spawn_detached(const char* prog, const char * const*argv)
{

#ifdef _WIN32
    if (prog==nullptr || argv==nullptr) {
        return false;
    }

    char full_path[_MAX_PATH];
    if( _fullpath(full_path, prog, _MAX_PATH ) != NULL ) {

        size_t argv_size=0;
        for (size_t i=0; argv[i] != nullptr; i++) {
            argv_size++;
        }

        const char** argv_with_prog = new const char*[argv_size+2];
        argv_with_prog[0] = full_path;

        for (size_t i=0; i!=argv_size; i++) {
            argv_with_prog[i+1] = argv[i];
        }
        argv_with_prog[argv_size+1] = nullptr;
        auto ret = _spawnv(P_DETACH, prog, argv_with_prog) != -1;
        delete [] argv_with_prog;
        return ret;
    }

    return false;
#else

    // 1. we fork to obtain an inbetween process
    // 2. the inbetween child process gets a new terminal and makes a pipe
    // 3. fork again to get the grandchild process
    // 4. grandchild copies args to force copy, since copy-on-write is not very useful here,
    //    as the pages need to be copied before the execv, in order to be able to signal
    //    the inbetween child process to exit. There are other ways to achieve the same effect.
    // 5. the inbetween child process reads the pipe and exits with pid of the grandchild or -1
    //    in case of error. The grandchild is orphaned (this is to prevent zombification).
    // 6. the parent waits the inbetween process and returns.

    // yes, all that (because, Linux...)

    #define IGNORE_SIGPIPE\
        struct sigaction signal_ignored;\
        memset(&signal_ignored, 0, sizeof(signal_ignored));\
        signal_ignored.sa_handler = SIG_IGN;\
        ::sigaction(SIGPIPE, &signal_ignored, 0);

    int rv = 0;
    pid_t child_pid = fork();
    if (child_pid == 0) {

        IGNORE_SIGPIPE
        ::setsid();

        int ready_pipe[2];
        pipe2(ready_pipe, O_CLOEXEC);

        pid_t grandchild_pid = fork();

        if (grandchild_pid == 0) {
            IGNORE_SIGPIPE
            close(ready_pipe[0]);

            // copy argv, to be no longer dependent on pages that have not been copied yet
            auto prog_copy = strdup(prog);
            int argc = 0;
            while (argv[argc]) { argc++; }
            auto argv_copy = new char* [argc+1]; // +1 for null terminator
            argv_copy[argc] = nullptr;
            for (int i = 0; i < argc; i++) {
                argv_copy[i] = strdup(argv[i]);
            }

            // allow the parent (child) process to exit
            write(ready_pipe[1], &rv, sizeof(int));                     // read in (1)

            // proceed with the new program
            ::execv(prog_copy, (char* const*)argv_copy);

            free(prog_copy);
            for (int i = 0; i < argc; i++) {
                free(argv_copy[i]);
            }
            delete[] argv_copy;

            // execv failed
            rv = -1;
            write(ready_pipe[1], &rv, sizeof(int));                     // read in (1)
            close(ready_pipe[1]);
            ::_exit(1);
        }
        else
        if (grandchild_pid == -1) {
            // second fork failed
            IGNORE_SIGPIPE
            rv = -1;
        }
        close(ready_pipe[1]);
        read(ready_pipe[0], &rv, sizeof(int));                          // (1)
        close(ready_pipe[0]);
        ::_exit(rv == 0? grandchild_pid : rv);
    }

    if (child_pid == -1) {
        // first fork failed
        return false;
    }

    ::waitpid(child_pid, &rv, 0);
    return rv == 0;

    #undef IGNORE_SIGPIPE

#endif // !defined(_WIN32)

}



struct Instantiator
{
    Instantiator(std::function<void()>&& deinstantiator):
        m_deinstantiator(deinstantiator)
    {}

    ~Instantiator()
    {
        m_deinstantiator();
    }

    std::function<void()> m_deinstantiator;
};



/*

// returns the binary image name of the running process (without the path)
std::string get_current_process_binary_name();



// tries to kill the processes whose pids are supplied in 'processes'
// returns the processes that could not be killed
std::unordered_set<DWORD> kill_processes(const std::unordered_set<DWORD> &processes);








inline
std::string get_current_process_binary_name()
{
    std::string ret;
    char name[MAX_PATH];
    HANDLE ph = OpenProcess(PROCESS_ALL_ACCESS, FALSE, GetCurrentProcessId());
    if (GetProcessImageFileNameA(ph, name, sizeof(name) / sizeof(*name)))
    {
        ret = std::string(name);
    }
    CloseHandle(ph);
    size_t backslash_pos = ret.find_last_of('\\');
    if (backslash_pos != ret.npos)
        ret = ret.substr(backslash_pos + 1);
    return ret;
}


inline
std::unordered_set<DWORD> kill_processes(const std::unordered_set<DWORD> &processes)
{
    std::unordered_set<DWORD> ret = processes;
    for (const auto& elem : processes) {
        HANDLE ph = OpenProcess(PROCESS_ALL_ACCESS, FALSE, elem);
        if (TerminateProcess(ph, 5))
            ret.erase(elem);
        CloseHandle(ph);
    }
    return ret;
}




//inline
//std::unordered_set<DWORD" find_running_processes_with_prefix(const wchar_t* prefix)
//{
//    std::unordered_set<DWORD" ret;
//    PROCESSENTRY32W entry;
//    entry.dwSize = sizeof(PROCESSENTRY32W);
//    int prefix_len = wcslen(prefix);
//
//    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
//
//    if (Process32FirstW(snapshot, &entry) == TRUE) {
//        while (Process32NextW(snapshot, &entry) == TRUE) {
//            if (wcsncmp(entry.szExeFile, prefix, prefix_len) == 0) {
//                ret.insert(entry.th32ProcessID);
//            }
//        }
//    }
//    CloseHandle(snapshot);
//    return ret;
//}


*/


}



#endif
