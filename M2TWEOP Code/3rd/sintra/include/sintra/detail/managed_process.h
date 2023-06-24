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

#ifndef SINTRA_MANAGED_PROCESS_H
#define SINTRA_MANAGED_PROCESS_H

#include "config.h"
#include "globals.h"
#include "ipc_rings.h"
#include "message.h"
#include "process_message_reader.h"
#include "resolve_type.h"
#include "spinlocked_containers.h"
#include "transceiver.h"
#include "utility/call_function_with_fusion_vector_args.h"

#include <algorithm>
#include <atomic>
#include <cassert>
#include <chrono>
#include <condition_variable>
#include <csignal>
#include <deque>
#include <float.h>
#include <list>
#include <map>
#include <mutex>
#include <stdexcept>
#include <string>
#include <thread>
#include <type_traits>

#include <boost/atomic.hpp>
#include <boost/interprocess/detail/os_thread_functions.hpp>


#undef max



namespace sintra {


using std::atomic_flag;
using std::condition_variable;
using std::deque;
using std::function;
using std::list;
using std::map;
using std::mutex;
using std::string;
using std::thread;
using std::vector;


inline
string uibs_impl(const char *file, int line)
{
    return std::string(file) + ":" + std::to_string(line);
};

#define UIBS uibs_impl(__FILE__, __LINE__)



struct Entry_descriptor
{
    Entry_descriptor(const string& binary_name)
    {
        m_binary_name = binary_name;
        assert(m_entry_function == nullptr);
    }
    Entry_descriptor(int(*entry_function)())
    {
        m_entry_function = entry_function;
        assert(m_binary_name.empty());
    }
private:
    int (*m_entry_function)() = nullptr;
    string m_binary_name;

    friend struct Managed_process;
};


struct Process_descriptor
{
    Entry_descriptor entry;
    vector<string> sintra_options;
    vector<string> user_options;
    int num_children = 0; // quota
    instance_id_type assigned_instance_id = invalid_instance_id;

    Process_descriptor(
        const Entry_descriptor& aentry,
        const vector<string>& auser_options = vector<string>(),
        int anum_children = 0)
    :
        entry(aentry),
        user_options(auser_options),
        num_children(anum_children)
    {
    }
    Process_descriptor(
        const string& binary_path,
        const vector<string>& auser_options = vector<string>(),
        int anum_children = 0)
    :
        entry(binary_path),
        user_options(auser_options),
        num_children(anum_children)
    {
    }
    Process_descriptor(
        const char* binary_path,
        const vector<string>& auser_options = vector<string>(),
        int anum_children = 0)
    :
        entry(binary_path),
        user_options(auser_options),
        num_children(anum_children)
    {
    }

    Process_descriptor(int(*entry_function)())
    :
        entry(entry_function)
    {
    }

    friend struct Managed_process;
};


// Branch indices have the following meaning:
// -1: No branching has taken place - this variable is not relevant
//  0: The starter process, with the coordinator.
// >0: A spawned process.
static inline int32_t s_branch_index = -1;


static inline uint32_t s_recovery_occurrence = 0;



template <typename T>
sintra::type_id_type get_type_id();


template <typename = void>
sintra::instance_id_type get_instance_id(string&& assigned_name);


struct Managed_process: Derived_transceiver<Managed_process>
{
    Managed_process();
    ~Managed_process();


    void init(int argc, const char* const* argv);
    bool branch(vector<Process_descriptor>& branch_vector);
    void go();

    // Pauses the process. Once called, reader threads will continue running,
    // but in a mode where only messages originating from the coordinator are
    // processed.
    void pause();

    // Stops the readers and causes their threads to exit.
    void stop();


    void wait_for_stop();

    void enable_recovery();


    Message_ring_W*                     m_out_req_c = nullptr;
    Message_ring_W*                     m_out_rep_c = nullptr;

    spinlocked_umap<
        string,
        type_id_type
    >                                   m_type_id_of_type_name;

    spinlocked_umap<
        string,
        instance_id_type
    >                                   m_instance_id_of_assigned_name;

    spinlocked_umap<
        instance_id_type,
        Transceiver*
    >                                   m_local_pointer_of_instance_id;

    // START/STOP

    enum Communication_state
    {
        COMMUNICATION_STOPPED,  // ring threads are stopped
        COMMUNICATION_PAUSED,   // rings answer to SERVICE_MODE
        COMMUNICATION_RUNNING   // rings in NORMAL_MODE
    };

    Communication_state                 m_communication_state = COMMUNICATION_STOPPED;
    mutex                               m_start_stop_mutex;
    condition_variable                  m_start_stop_condition;


    Managed_process(Managed_process const&) = delete;
    void operator=(Managed_process const&)  = delete;


    string                              m_binary_name;

    ipc::ipcdetail::OS_process_id_t     m_pid;

    atomic<bool>                        m_must_stop;
    condition_variable                  m_termination_condition;

    uint64_t                            m_swarm_id;
    string                              m_directory;

    inline
    string obtain_swarm_directory();

    function<int()> m_entry_function = [&] ()->int { return 0; };
    int                                 m_children_quota = 0;

    sequence_counter_type               m_last_message_sequence;

    // don't be tempted to make this atomic and shared across processes, it's not faster.
    sequence_counter_type               m_check_sequence;

    double                              m_message_stats_reference_time;
    uint64_t                            m_messages_accepted_since_reference_time;
    uint64_t                            m_messages_rejected_since_reference_time;
    sequence_counter_type               m_total_sequences_missed;

    std::chrono::time_point<std::chrono::system_clock>
                                        m_time_instantiated;

    map<
        instance_id_type,
        Process_message_reader
    >                                   m_readers;

    int                                 m_num_active_readers = 0;
    mutex                               m_num_active_readers_mutex;
    condition_variable                  m_num_active_readers_condition;
    void wait_until_all_external_readers_are_done();

    void flush(instance_id_type process_id, sequence_counter_type flush_sequence);


    size_t unblock_rpc(instance_id_type process_instance_id = invalid_instance_id);


    // This signal will be sent BEFORE the coordinator sends instance_unpublished
    // for this process. It is meant to notify crash guards about the reason of the
    // instance_unpublished event, which will follow shortly after.
    SINTRA_SIGNAL_EXPLICIT(terminated_abnormally, int status);

    spinlocked_umap<tn_type, list<function<void()>>>
                                        m_queued_availability_calls;

    mutex                               m_availability_mutex;

    // TODO: FIXME: The recursive can be easily avoided. Implement an additional
    // activation path.
    recursive_mutex                     m_handlers_mutex;

    // Calls f when the specified transceiver becomes available.
    // if the transceiver is available, f is invoked immediately.
    template <typename T>
    function<void()> call_on_availability(Named_instance<T> transceiver, function<void()> f);


    deque<sequence_counter_type>        m_flush_sequence;
    mutex                               m_flush_sequence_mutex;
    condition_variable                  m_flush_sequence_condition;

    handler_registry_type               m_active_handlers;

    // standard process groups
    instance_id_type                    m_group_all      = invalid_instance_id;
    instance_id_type                    m_group_external = invalid_instance_id;

    // recovery
    double                              m_average_runnning_time = std::numeric_limits<double>::max();
    double                              m_last_running_time     = std::numeric_limits<double>::max();
    uint32_t                            m_times_recovered       = 0;
    bool                                m_recoverable           = false;
    std::string                         m_recovery_cmd;


    struct Spawn_swarm_process_args
    {
        std::string                 binary_name;
        std::vector<std::string>    args;
        instance_id_type            piid;
        uint32_t                    occurrence = 0;
    };


    bool spawn_swarm_process( const Spawn_swarm_process_args& ssp_args );

    map<instance_id_type, Spawn_swarm_process_args> m_cached_spawns;
};


} // namespace sintra


#endif
