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

#ifndef SINTRA_MANAGED_PROCESS_IMPL_H
#define SINTRA_MANAGED_PROCESS_IMPL_H


#include "utility.h"

#include <chrono>
#include <mutex>

#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/join.hpp>
#include <boost/type_index/ctti_type_index.hpp>

#ifdef _WIN32
    #include "third_party/getopt.h"
#else
    #include <getopt.h>
#endif


namespace sintra {



inline
static void s_signal_handler(int sig)
{
    if (!s_mproc->m_out_req_c) {
        // This means that we crashed before even having been able to initialize.
        // We have no rings to emit to, so that's all there is to do.
        // This code should ideally be unreachable.
        assert(false);
    }
    else {
        s_mproc->emit_remote<Managed_process::terminated_abnormally>(sig);

        for (auto& reader : s_mproc->m_readers) {
            // waiting here is pointless, we are single-threaded
            reader.second.stop_nowait();
        }
    }

    raise(sig);
}



inline
void install_signal_handler()
{
    // synchronous (traps)
    signal(SIGABRT, s_signal_handler);
    signal(SIGFPE,  s_signal_handler);
    signal(SIGILL,  s_signal_handler);
    signal(SIGSEGV, s_signal_handler);

    // asynchronous
    signal(SIGINT, s_signal_handler);
    signal(SIGTERM, s_signal_handler);
}



template <typename T>
sintra::type_id_type get_type_id()
{
    const std::string pretty_name = boost::typeindex::ctti_type_index::template type_id<T>().pretty_name();
    auto it = s_mproc->m_type_id_of_type_name.find(pretty_name);
    if (it != s_mproc->m_type_id_of_type_name.end()) {
        return it->second;
    }

    // Caution the Coordinator call will refer to the map that is being assigned,
    // if the Coordinator is local. Do not be tempted to simplify the temporary,
    // because depending on the order of evaluation, it may or it may not work.
    auto tid = Coordinator::rpc_resolve_type(s_coord_id, pretty_name);

    // if it is not invalid, cache it
    if (tid != invalid_type_id) {
        s_mproc->m_type_id_of_type_name[pretty_name] = tid;
    }

    return tid;
}

// helper
template <typename T>
sintra::type_id_type get_type_id(const T&) {return get_type_id<T>();}


template <typename>
sintra::instance_id_type get_instance_id(std::string&& assigned_name)
{
    auto it = s_mproc->m_instance_id_of_assigned_name.find(assigned_name);
    if (it != s_mproc->m_instance_id_of_assigned_name.end()) {
        return it->second;
    }

    // Caution the Coordinator call will refer to the map that is being assigned,
    // if the Coordinator is local. Do not be tempted to simplify the temporary,
    // because depending on the order of evaluation, it may or it may not work.
    auto iid = Coordinator::rpc_resolve_instance(s_coord_id, assigned_name);

    // if it is not invalid, cache it
    if (iid != invalid_instance_id) {
        s_mproc->m_instance_id_of_assigned_name[assigned_name] = iid;
    }

    return iid;
}







inline
Managed_process::Managed_process():
    Derived_transceiver<Managed_process>((void*)0),
    m_communication_state(COMMUNICATION_STOPPED),
    m_must_stop(false),
    m_swarm_id(0),
    m_last_message_sequence(0),
    m_check_sequence(0),
    m_message_stats_reference_time(0.),
    m_messages_accepted_since_reference_time(0),
    m_messages_rejected_since_reference_time(0),
    m_total_sequences_missed(0)
{
    assert(s_mproc == nullptr);
    s_mproc = this;

    m_pid = ipc::ipcdetail::get_current_process_id();

    install_signal_handler();

    // NOTE: Do not be tempted to use get_current_process_creation_time from boost::interprocess,
    // it is only implemented for Windows.
    m_time_instantiated = std::chrono::system_clock::now();
}



inline
Managed_process::~Managed_process()
{
    // the coordinating process will be removing its readers whenever
    // they are unpublished - when they are all done, the process may exit
    if (s_coord) {
        wait_until_all_external_readers_are_done();
    }

    assert(m_communication_state <= COMMUNICATION_PAUSED); // i.e. paused or stopped

    // this is called explicitly, in order to inform the coordinator of the destruction early.
    // it would not be possible to communicate it after the channels were closed.
    this->Derived_transceiver<Managed_process>::destroy();

    // no more reading
    m_readers.clear();

    // no more writing
    if (m_out_req_c) {
        delete m_out_req_c;
        m_out_req_c = nullptr;
    }

    if (m_out_rep_c) {
        delete m_out_rep_c;
        m_out_rep_c = nullptr;
    }

    if (s_coord) {

        // now it's safe to delete the Coordinator.
        delete s_coord;
        s_coord = 0;

        // removes the swarm directory
        remove_directory(m_directory);
    }

    s_mproc = nullptr;
    s_mproc_id = 0;
}



// returns the argc/argv as a vector of strings
inline
std::vector<std::string> argc_argv_to_vector(int argc, const char* const* argv)
{
    std::vector<std::string> ret;
    for (int i = 0; i < argc; i++) {
        ret.push_back(argv[i]);
    }
    return ret;
}



struct Filtered_args
{
    vector<string> remained;
    vector<string> extracted;
};



inline
Filtered_args filter_option(
    std::vector<std::string> in_args, std::string in_option, unsigned int num_args
)
{
    Filtered_args ret;
    bool found = false;
    for (auto& e : in_args) {
        if (!found) {
            if (e == in_option) {
                found = true;
                ret.extracted.push_back(e);
                continue;
            }
            ret.remained.push_back(e);
        }
        else {
            if (num_args) {
                ret.extracted.push_back(e);
                num_args--;
            }
            else {
                ret.remained.push_back(e);
            }
        }
    }
    return ret;
}



inline
void Managed_process::init(int argc, const char* const* argv)
{
    m_binary_name = argv[0];

    int help_arg = 0;
    std::string branch_index_arg;
    std::string swarm_id_arg;
    std::string instance_id_arg;
    std::string coordinator_id_arg;
    std::string recovery_arg;

    size_t recovery_occurrence_value = 0;
    auto fa = filter_option(argc_argv_to_vector(argc, argv), "--recovery_occurrence", 1);

    if (fa.extracted.size() == 2) {
        try {
            recovery_occurrence_value =  std::stoul(fa.extracted[1]);
        }
        catch (...) {
            assert(!"not implemented");
        }
    }

    m_recovery_cmd = boost::algorithm::join(fa.remained, " ") + " --recovery_occurrence " +
        std::to_string(recovery_occurrence_value+1);

    try {
        while (true) {
            static struct ::option long_options[] = {
                {"help",                no_argument,        &help_arg,  'h' },
                {"branch_index",        required_argument,  0,          'a' },
                {"swarm_id",            required_argument,  0,          'b' },
                {"instance_id",         required_argument,  0,          'c' },
                {"coordinator_id",      required_argument,  0,          'd' },
                {"recovery_occurrence", required_argument,  0,          'e' },
                {0, 0, 0, 0}
            };

            int option_index = 0;
            int c = getopt_long(argc, (char*const*)argv, "ha:b:c:d:e:", long_options, &option_index);

            if (c == -1)
                break;

            switch (c) {
                case 'h':
                    if (long_options[option_index].flag != 0)
                        throw -1;
                    break;
                case 'a':
                    branch_index_arg        = optarg;
                    s_branch_index          = boost::lexical_cast<decltype(s_branch_index)>(optarg);
                    if (s_branch_index < 1) {
                        throw -1;
                    }
                    break;
                case 'b':
                    swarm_id_arg            = optarg;
                    m_swarm_id              = boost::lexical_cast<decltype(m_swarm_id)>(optarg);
                    break;
                case 'c':
                    instance_id_arg         = optarg;
                    m_instance_id           = boost::lexical_cast<decltype(m_instance_id)>(optarg);
                    break;
                case 'd':
                    coordinator_id_arg      = optarg;
                    s_coord_id              = boost::lexical_cast<decltype(s_coord_id)>(optarg);
                    break;
                case 'e':
                    recovery_arg            = optarg;
                    s_recovery_occurrence   = boost::lexical_cast<decltype(s_recovery_occurrence)>(optarg);
                    break;
                case '?':
                    /* getopt_long already printed an error message. */
                    break;
                default :
                    throw -1;
            }
        }
    }
    catch(...) {
        cout << R"(
Managed process options:
  --help                   (optional) produce help message and exit
  --branch_index arg       used by the coordinator process, when it invokes
                           itself
                           with a different entry index. It must be 1 or
                           greater.
  --swarm_id arg           unique identifier of the swarm that is being joined
  --instance_id arg        the instance id assigned to the new process
  --coordinator_id arg     the instance id of the coordinator that this
                           process should refer to
  --recovery_occurrence arg (optional) number of times the process recovered an
                           abnormal termination.
)";
        exit(1);
    }

    bool coordinator_is_local = false;
    if (swarm_id_arg.empty()) {
        s_mproc_id = m_instance_id = make_process_instance_id();

        m_swarm_id = 
            std::chrono::duration_cast<std::chrono::nanoseconds>(
                m_time_instantiated.time_since_epoch()
            ).count();
        coordinator_is_local = true;

        // NOTE: leave s_branch_index uninitialized. The coordinating process does
        // not have an entry
    }
    else {
        if (coordinator_id_arg.empty() || (branch_index_arg.empty() && instance_id_arg.empty()) ) {

            assert(!"if the binary was not invoked manually, this is definitely a bug.");
            exit(1);
        }

        if (!branch_index_arg.empty()) {
            assert(s_branch_index < max_process_index - 1);
        }

        assert(m_instance_id != 0);

        if (instance_id_arg.empty()) {
            // If branch_index is specified, the explicit instance_id may not, thus
            // we need to make a process instance id based on the branch_index.
            // Transceiver IDs start from 2 (0 is invalid, 1 is the first process)
            m_instance_id = make_process_instance_id(s_branch_index + 2);
        }

        s_mproc_id = m_instance_id;
    }
    m_directory = obtain_swarm_directory();

    m_out_req_c = new Message_ring_W(m_directory, "req", m_instance_id);
    m_out_rep_c = new Message_ring_W(m_directory, "rep", m_instance_id);

    if (coordinator_is_local) {
        s_coord = new Coordinator;
        s_coord_id = s_coord->m_instance_id;

        {
            lock_guard<mutex> lock(s_coord->m_publish_mutex);
            s_coord->m_transceiver_registry[s_mproc_id];
        }
    }

    assert(!m_readers.count(process_of(s_coord_id)));
    auto it = m_readers.emplace(process_of(s_coord_id), process_of(s_coord_id));
    assert(it.second == true);
    it.first->second.wait_until_ready();

    // Up to this point, there was no infrastructure for a proper construction
    // of Transceiver base.

    this->Derived_transceiver<Managed_process>::construct("", m_instance_id);

    auto published_handler = [this](const Coordinator::instance_published& msg)
    {
        tn_type tn = {msg.type_id, msg.assigned_name};
        lock_guard<mutex> lock(m_availability_mutex);

        auto it = m_queued_availability_calls.find(tn);
        if (it != m_queued_availability_calls.end()) {
            while (!it->second.empty()) {
                // each function call, which is a lambda defined inside 
                // call_on_availability(), clears itself from the list as well.
                it->second.front()();
            }
            m_queued_availability_calls.erase(it);
        }
    };


    auto unpublished_handler = [this](const Coordinator::instance_unpublished& msg)
    {
        auto iid = msg.instance_id;
        auto process_iid = process_of(iid);
        if (iid == process_iid) {

            // the unpublished transceiver was a process, thus we should
            // remove all transceiver records who are known to live in it.

            for (auto it = m_instance_id_of_assigned_name.begin(); it != m_instance_id_of_assigned_name.end();) {
                if (process_of(it->second) == iid) {
                    it = m_instance_id_of_assigned_name.erase(it);
                }
                else {
                    ++it;
                }
            }

            s_mproc->unblock_rpc(iid);
        }

        // if the unpublished transceiver is the coordinator process, we have to stop.
        if (process_of(s_coord_id) == msg.instance_id) {
            stop();
            exit(0);
        }
    };

    if (!s_coord) {
        activate(published_handler,   Typed_instance_id<Coordinator>(s_coord_id));
        activate(unpublished_handler, Typed_instance_id<Coordinator>(s_coord_id));
    }

    if (coordinator_is_local) {
        auto cr_handler = [](const Managed_process::terminated_abnormally& msg)
        {
            s_coord->unpublish_transceiver(msg.sender_instance_id);

            s_coord->recover_if_required(msg.sender_instance_id);

            /*
            
            There is a problem here:
            We reach this code in the ring reader of the process that crashed.
            The message is then relayed to the coordinator ring
            but the coordinating process reads its own ring too
            which will cause this to be handled twice... which is wrong

            a fix would be to simply reject the message here, in one of the two cases
            but this is not the only case that such a problem could occur. It thus needs a better solution.
            Also, this solution would require declaring some thread-local stuff, otherwise
            knowing whether we are reading the coordinator is impossible.

            maybe an alternative would be that the coordinating process does not handle such messages
            unless they are originating from its own ring. This would however require this to be checked
            in the message loop, which would make it slower for ALL processes - maybe not a good idea.

            another possible alternative (that requires more thought) is that the coordinating process
            does not read its own ring. There might be several corner cases which won't work.

            */

        };
        activate<Managed_process>(cr_handler, any_remote);
    }
    else {
        auto cr_handler = [](const Managed_process::terminated_abnormally& msg)
        {
            // if the unpublished transceiver is the coordinator process, we have to stop.
            if (process_of(s_coord_id) == msg.sender_instance_id) {
                s_mproc->stop();
                exit(1);
            }
        };
        activate<Managed_process>(cr_handler, any_remote);
    }

    m_start_stop_mutex.lock();

    m_communication_state = COMMUNICATION_RUNNING;
    m_start_stop_mutex.unlock();
}



inline
bool Managed_process::spawn_swarm_process(
    const Spawn_swarm_process_args& s )
{
    assert(s_coord);
    std::vector<instance_id_type> failed_spawns, successful_spawns;
    auto args = s.args;
    args.insert(args.end(), {"--recovery_occurrence", std::to_string(s.occurrence)} );

    cstring_vector cargs(args);

    assert(!m_readers.count(process_of(s.piid)));
    auto eit = m_readers.emplace(s.piid, s.piid);
    assert(eit.second == true);

    // Before spawning the new process, we have to assure that the
    // corresponding reading threads are up and running.
    eit.first->second.wait_until_ready();

    bool success = spawn_detached(s.binary_name.c_str(), cargs.v());

    if (success) {
        // Create an entry in the coordinator's transceiver registry.
        // This is essential for the implementation of publish_transceiver()
        {
            lock_guard<mutex> lock(s_coord->m_publish_mutex);
            s_coord->m_transceiver_registry[s.piid];
        }

        // create the readers. The next line will start the reader threads,
        // which might take some time. At this stage, we do not have to wait
        // until they are ready for messages.
        //m_readers.emplace_back(std::move(reader));

        m_cached_spawns[s.piid] = s;
        m_cached_spawns[s.piid].occurrence++;
    }
    else {
        std::cerr << "failed to launch " << s.binary_name << std::endl;

        //m_readers.pop_back();
        m_readers.erase(s.piid);
    }

    return success;
}



inline
bool Managed_process::branch(vector<Process_descriptor>& branch_vector)
{
    // this function may only be called when a group of processes start.
    assert(!branch_vector.empty());

    using namespace sintra;
    using std::to_string;

    if (s_coord) {

        // 1. prepare the command line for each invocation
        auto it = branch_vector.begin();
        for (int i = 1; it != branch_vector.end(); it++, i++) {

            auto& options = it->sintra_options;
            if (it->entry.m_binary_name.empty()) {
                it->entry.m_binary_name = m_binary_name;
                options.insert(options.end(), { "--branch_index", to_string(i) });
            }
            options.insert(options.end(), {
                "--swarm_id",       to_string(m_swarm_id),
                "--instance_id",    to_string(it->assigned_instance_id = make_process_instance_id()),
                "--coordinator_id", to_string(s_coord_id)
            });
        }

        // 2. spawn
        std::unordered_set<instance_id_type> successfully_spawned;
        it = branch_vector.begin();
        //auto readers_it = m_readers.begin();
        for (int i = 0; it != branch_vector.end(); it++, i++) {

            std::vector<std::string> all_args = {it->entry.m_binary_name.c_str()};
            all_args.insert(all_args.end(), it->sintra_options.begin(), it->sintra_options.end());
            all_args.insert(all_args.end(), it->user_options.begin(), it->user_options.end());

            if (spawn_swarm_process({it->entry.m_binary_name, all_args, it->assigned_instance_id})) {
                successfully_spawned.insert(it->assigned_instance_id);
            }
        }

        auto all_processes = successfully_spawned;
        all_processes.insert(m_instance_id);

        m_group_all      = s_coord->make_process_group("_sintra_all_processes", all_processes);
        m_group_external = s_coord->make_process_group("_sintra_external_processes", successfully_spawned);

        s_branch_index = 0;
    }
    else {
        assert(s_branch_index != -1);
        assert( (ptrdiff_t)branch_vector.size() > s_branch_index-1);
        Process_descriptor& own_pd = branch_vector[s_branch_index-1];
        if (own_pd.entry.m_entry_function != nullptr) {
            m_entry_function = own_pd.entry.m_entry_function;
        }
        else {
            assert(!"if the binary was not invoked manually, this is definitely a bug.");
            exit(1);
        }

        m_group_all      = Coordinator::rpc_wait_for_instance(s_coord_id, "_sintra_all_processes");
        m_group_external = Coordinator::rpc_wait_for_instance(s_coord_id, "_sintra_external_processes");
    }

    // assign_name requires that all group processes are instantiated, in order
    // to receive the instance_published event
    bool all_started = Process_group::rpc_barrier(m_group_all, UIBS);
    if (!all_started) {
        return false;
    }

    return true;
}


inline
void Managed_process::go()
{
    assign_name(std::string("sintra_process_") + std::to_string(m_pid));

    m_entry_function();

    // Calling deactivate_all() is definitely wrong for the coordinator process.
    // For the rest, it is probably harmless.
    // The point of calling this here is to prevent running a handler
    // while the process is not in a state capable of running a handler (e.g. during destruction).
    // It is the responsibility of the library user to handle this situation properly, but
    // there might just be too many expectations from the user.
    if (!s_coord) {
        s_mproc->deactivate_all();
    }
}



 //////////////////////////////////////////////////////////////////////////
///// BEGIN START/STOP /////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
//////   \//////   \//////   \//////   \//////   \//////   \//////   \//////
 ////     \////     \////     \////     \////     \////     \////     \////
  //       \//       \//       \//       \//       \//       \//       \//



inline
void Managed_process::pause()
{
    std::lock_guard<mutex> start_stop_lock(m_start_stop_mutex);

    // pause() might be called when the entry function finishes execution,
    // explicitly from one of the handlers, or from the entry function itself.
    // If called when the process is already paused, this should not have any
    // side effects.
    if (m_communication_state <= COMMUNICATION_PAUSED)
        return;

    for (auto& ri : m_readers) {
        ri.second.pause();
    }

    m_communication_state = COMMUNICATION_PAUSED;
    m_start_stop_condition.notify_all();
}


inline
void Managed_process::stop()
{
    std::lock_guard<mutex> start_stop_lock(m_start_stop_mutex);

    // stop() might be called explicitly from one of the handlers, or from the
    // entry function. If called when the process is already stopped, this
    // should not have any side effects.
    if (m_communication_state == COMMUNICATION_STOPPED)
        return;

    for (auto& ri : m_readers) {
        ri.second.stop_nowait();
    }

    m_communication_state = COMMUNICATION_STOPPED;
    m_start_stop_condition.notify_all();
}



inline
void Managed_process::wait_for_stop()
{
    std::unique_lock<mutex> start_stop_lock(m_start_stop_mutex);
    while (m_communication_state == COMMUNICATION_RUNNING) {
        m_start_stop_condition.wait(start_stop_lock);
    }
}


  //\       //\       //\       //\       //\       //\       //\       //
 ////\     ////\     ////\     ////\     ////\     ////\     ////\     ////
//////\   //////\   //////\   //////\   //////\   //////\   //////\   //////
////////////////////////////////////////////////////////////////////////////
///// END START/STOP ///////////////////////////////////////////////////////
 //////////////////////////////////////////////////////////////////////////



inline
std::string Managed_process::obtain_swarm_directory()
{
    std::string sintra_directory = fs::temp_directory_path().string() + "/sintra/";
    if (!check_or_create_directory(sintra_directory)) {
        throw std::runtime_error("access to a working directory failed");
    }

    std::stringstream stream;
    stream << std::hex << m_swarm_id;
    auto swarm_directory = sintra_directory + stream.str();
    if (!check_or_create_directory(swarm_directory)) {
        throw std::runtime_error("access to a working directory failed");
    }

    return swarm_directory;
}



// Calls f when the specified transceiver becomes available.
// if the transceiver is available, f is invoked immediately.
template <typename T>
function<void()> Managed_process::call_on_availability(Named_instance<T> transceiver, function<void()> f)
{
    lock_guard<mutex> lock(m_availability_mutex);

    auto iid = Typed_instance_id<T>(get_instance_id(std::move(transceiver)));

    //if the transceiver is available, call f and skip the queue
    if (iid.id != invalid_instance_id) {
        f();

        // it's done - there is nothing to disable, thus returning an empty function.
        return []() {};
    }

    tn_type tn = { get_type_id<T>(), transceiver };

    // insert an empty function, in order to be able to capture the iterator within it
    m_queued_availability_calls[tn].emplace_back();
    auto f_it = std::prev(m_queued_availability_calls[tn].end());

    // this is the abort call
    auto ret = Adaptive_function([this, tn, f_it]() {
        m_queued_availability_calls[tn].erase(f_it);
        });

    // and this is the actual call, which besides calling f, also neutralizes the
    // returned abort calls and deletes its entry from the call queue.
    *f_it = [this, f, ret, tn, f_it]() mutable {
        f();

        // neutralize abort calls. Calling abort using the function returned
        // by call_on_availability() from now on would have no effect.
        ret.set([]() {});

        // erase the current lambda from the list
        m_queued_availability_calls[tn].erase(f_it);
    };

    return ret;
}


inline
void Managed_process::wait_until_all_external_readers_are_done()
{
    unique_lock<mutex> lock(m_num_active_readers_mutex);
    while (m_num_active_readers > 2) {
        m_num_active_readers_condition.wait(lock);
    }
}


inline
void Managed_process::flush(instance_id_type process_id, sequence_counter_type flush_sequence)
{
    assert(is_process(process_id));

    auto it = m_readers.find(process_id);
    if (it == m_readers.end()) {
        throw std::logic_error(
            "attempted to flush the channel of a process which is not being read"
        );
    }
    auto& reader = it->second;
    auto rs = reader.get_request_reading_sequence();
    if (rs < flush_sequence) {
        std::unique_lock<mutex> flush_lock(m_flush_sequence_mutex);
        m_flush_sequence.push_back(flush_sequence);
        while (!m_flush_sequence.empty() &&
            m_flush_sequence.front() <= flush_sequence)
        {
            m_flush_sequence_condition.wait(flush_lock);
        }
    }
}


inline
size_t Managed_process::unblock_rpc(instance_id_type process_instance_id)
{
    assert(!process_instance_id || is_process(process_instance_id));
    size_t ret = 0;
    unique_lock<mutex> ol(s_outstanding_rpcs_mutex);
    if (!s_outstanding_rpcs.empty()) {

        for (auto& c : s_outstanding_rpcs) {
            unique_lock<mutex> il(c->keep_waiting_mutex);

            if (process_instance_id != invalid_instance_id &&
                process_of(c->remote_instance) == process_instance_id)
            {
                c->success = false;
                c->keep_waiting = false;
                c->keep_waiting_condition.notify_one();
                ret++;
            }
        }
    }
    return ret;
}


} // sintra


#endif
