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

#ifndef SINTRA_IMPL_H
#define SINTRA_IMPL_H


namespace sintra {


using std::ostringstream;
using std::string;
using std::vector;




inline
std::vector<Process_descriptor> make_branches(std::vector<Process_descriptor>& v)
{
    return v;
}


template <typename ...Args>
std::vector<Process_descriptor> make_branches(std::vector<Process_descriptor>& v, int multiplicity, Process_descriptor pd, Args&&... rest)
{
    for (int i=0; i<multiplicity; i++) {
        v.push_back(pd);
    }
    return make_branches(v, std::forward<Args>(rest)...);
}


template <typename ...Args>
std::vector<Process_descriptor> make_branches(int multiplicity, const Process_descriptor& pd, Args&&... rest)
{
    std::vector<Process_descriptor> v;
    return make_branches(v, multiplicity, pd, std::forward<Args>(rest)...);
}


template <typename ...Args>
std::vector<Process_descriptor> make_branches(std::vector<Process_descriptor>& v, Process_descriptor pd, Args&&... rest)
{
    v.push_back(pd);
    return make_branches(v, std::forward<Args>(rest)...);
}


template <typename ...Args>
std::vector<Process_descriptor> make_branches(const Process_descriptor& pd, Args&&... rest)
{
    std::vector<Process_descriptor> v;
    return make_branches(v, pd, std::forward<Args>(rest)...);
}



inline
bool finalize();



inline
void init(int argc, const char* const* argv, std::vector<Process_descriptor> v = std::vector<Process_descriptor>())
{

#ifndef _WIN32
    struct sigaction noaction;
    memset(&noaction, 0, sizeof(noaction));
    noaction.sa_handler = SIG_IGN;
    sigaction(SIGPIPE, &noaction, 0);
    setsid();
#endif

    static bool once = false;
    assert(!once); // init() may only be run once.
    once = true;

    struct Cleanup_guard
    {
        Cleanup_guard(std::function<void()> f) {m_f = f;}
        ~Cleanup_guard() {m_f();}
        std::function<void()> m_f;
    };

    static Cleanup_guard cleanup_guard([&]() {
        if (s_mproc) {
            //assert(!"sintra::finalize was not called!");
            finalize();
        }
    });

    s_mproc = new Managed_process;
    s_mproc->init(argc, argv);
    if (!v.empty()) {
        s_mproc->branch(v);
    }
    s_mproc->go();
}



template<typename... Args>
void init(int argc, const char* const* argv, Args&&... args)
{
#ifndef NDEBUG
    auto cache_line_size = get_cache_line_size();
    if (assumed_cache_line_size != cache_line_size) {
        std::cerr << "WARNING: assumed_cache_line_size is set to " << assumed_cache_line_size
            << ", but on this system it is actually " << cache_line_size << "." << std::endl;
    }
#endif

    init(argc, argv, make_branches(std::forward<Args>(args)...));
}



inline
bool finalize()
{
    if (!s_mproc) {
        // Sintra was not initialized, or it has already been finalized.
        // Either way, this call will have no effect.
        return false;
    }

    s_mproc->pause();
    delete s_mproc;
    s_mproc = nullptr;
    return true;
}



inline
size_t spawn_swarm_process(const std::string& binary_name, vector<string> args= {}, size_t multiplicity = 1)
{
    size_t ret = 0;
    auto piid = make_process_instance_id();

    args.insert(args.end(), {
        "--swarm_id",       std::to_string(s_mproc->m_swarm_id),
        "--instance_id",    std::to_string(piid),
        "--coordinator_id", std::to_string(s_coord_id)
    });

    Managed_process::Spawn_swarm_process_args spawn_args;
    spawn_args.binary_name = binary_name;
    spawn_args.args = args;

    for (size_t i=0; i<multiplicity; i++) {
        spawn_args.piid = piid;
        if (s_mproc->spawn_swarm_process(spawn_args)) {
            ret++;
        }
    }
    return ret;
}



inline
int process_index()
{
    return s_branch_index;
}


// This is a convenience function.
inline
bool barrier(const std::string& barrier_name, const std::string& group_name)
{
    auto flush_seq = Process_group::rpc_barrier(group_name, barrier_name);
    if (flush_seq == invalid_sequence) {
        return false;
    }

    // if the coordinator is not local, we must flush its channel
    // (i.e. all messages on the coordinator's channel must be processed before proceeding further)
    if (!s_coord) {
        s_mproc->flush(process_of(s_coord_id), flush_seq);
    }

    return true;
}


template <typename FT, typename SENDER_T>
auto activate_slot(const FT& slot_function, Typed_instance_id<SENDER_T> sender_id)
{
    return s_mproc->activate(slot_function, sender_id);
}


inline
void deactivate_all_slots()
{
    s_mproc->deactivate_all();
}



template <instance_id_type LOCALITY>
struct Maildrop
{
    using mp_type = Managed_process::Transceiver_type;

    // an array of char turns to an std::string.
    template <size_t N>
    Maildrop& operator << (const char(&value)[N])
    {
        using MT = Message<Enclosure<string>>;
        s_mproc->send<MT, LOCALITY, mp_type>(string(value));
        return *this;
    }

    // a c string turns to an std::string.
    Maildrop& operator << (const char* value)
    {
        using MT = Message<Enclosure<string>>;
        s_mproc->send<MT, LOCALITY, mp_type>(string(value));
        return *this;
    }

    // an array of anything other than a char turns to a vector
    template <size_t N, typename T>
    Maildrop& operator << (const T(&v)[N])
    {
        using MT = Message<Enclosure<vector<T>>>;
        s_mproc->send<MT, LOCALITY, mp_type>(vector<T>(v, v + sizeof v / sizeof *v));
        return *this;
    }

    template <typename T>
    Maildrop& operator << (const T& value)
    {
        using MT = Message<Enclosure<T>>;
        s_mproc->send<MT, LOCALITY, mp_type>(value);
        return *this;
    }
};


inline Maildrop<any_local>&           local()  { static Maildrop<any_local>           m; return m; }
inline Maildrop<any_remote>&          remote() { static Maildrop<any_remote>          m; return m; }
inline Maildrop<any_local_or_remote>& world()  { static Maildrop<any_local_or_remote> m; return m; }



struct Console
{
    Console(){}
    Console(const Console&)             = delete;
    Console& operator=(const Console&)  = delete;

    ~Console()
    {
        try {
            Coordinator::rpc_print(s_coord_id, m_oss.str());
        }
        catch (...) {
            // failure to print, is silent.
        }
    }

    template <typename T>
    Console& operator << (const T& value)
    {
        m_oss << value;
        return *this;
    }

private:
    ostringstream m_oss;

    // prevent heap instances
    void* operator new(size_t);
    void* operator new(size_t, void*);
    void* operator new[](size_t);
    void* operator new[](size_t, void*);
};


using console = Console;


} // namespace sintra



#endif
