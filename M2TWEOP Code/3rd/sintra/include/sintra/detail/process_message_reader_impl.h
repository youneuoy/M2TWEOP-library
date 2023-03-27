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

#ifndef SINTRA_PROCESS_MESSAGE_READER_IMPL_H
#define SINTRA_PROCESS_MESSAGE_READER_IMPL_H


#include "transceiver_impl.h"
#include <memory>

namespace sintra {


using std::thread;
using std::unique_ptr;


inline bool thread_local tl_is_req_thread = false;

// This did not work correctly on mingw 11.2.0, thus we shall not use inline thread_local
// non-POD objects as they seem to be poorly implemented (January 2022).
// The problem here was related to the library implementation, not to the compiler.
//inline thread_local function<void()> tl_post_handler_function = nullptr;

// We can use a thread_local raw pointer instead.
inline thread_local function<void()>* tl_post_handler_function = nullptr;

inline
Process_message_reader::Process_message_reader(instance_id_type process_instance_id):
    m_reader_state(READER_NORMAL),
    m_process_instance_id(process_instance_id)
{
    m_in_req_c = new Message_ring_R(s_mproc->m_directory, "req", m_process_instance_id);
    m_in_rep_c = new Message_ring_R(s_mproc->m_directory, "rep", m_process_instance_id);
    m_request_reader_thread = new thread([&] () { request_reader_function(); });
    m_request_reader_thread->detach();
    m_reply_reader_thread   = new thread([&] () { reply_reader_function();   });
    m_reply_reader_thread->detach();
}


inline
void Process_message_reader::wait_until_ready()
{
    while (!m_req_running) {}
    while (!m_rep_running) {}
}



inline
void Process_message_reader::stop_nowait()
{    
    m_reader_state = READER_STOPPING;

    m_in_req_c->done_reading();
    m_in_req_c->unblock_local();

    // if there are outstanding RPC calls waiting for reply, they should be
    // unblocked (and fail), to avoid a deadlock
    s_mproc->unblock_rpc();


    if (!tl_is_req_thread) {
        m_in_rep_c->done_reading();
        m_in_rep_c->unblock_local();
    }
    else {
        // The purpose of the lambda below is the following scenario:
        // 1. This function is called from a handler, thus from within the request
        //    reading function.
        // 2. The very same handler also calls some RPC, whose reply is sent through
        //    the reply ring, in the corresponding thread.
        // If we force the reply ring to exit, there will be no ring to get the
        // reply from, leading to a deadlock. Thus if the function is determined
        // to be called from within the request reading thread, forcing the reply
        // reading thread to exit will happen only after the request reading loop
        // exits.

        if (tl_post_handler_function) {
            delete tl_post_handler_function;
        }

        tl_post_handler_function = new
            function<void()>([this]() {
                m_in_rep_c->done_reading();
                m_in_rep_c->unblock_local();
            }
        );
    }
}



inline
bool Process_message_reader::stop_and_wait(double waiting_period)
{
    std::unique_lock<std::mutex> lk(m_stop_mutex);
    stop_nowait();

    m_stop_condition.wait_for(lk, std::chrono::duration<double>(waiting_period),
        [&]() { return !(m_req_running || m_rep_running); }
    );

    if (m_req_running || m_rep_running) {
        // We might get here, if the coordinator is gone already.
        // In this case, we unblock pending RPC calls and do some more waiting.
        s_mproc->unblock_rpc(m_process_instance_id);
        m_stop_condition.wait_for(lk, std::chrono::duration<double>(waiting_period),
            [&]() { return !(m_req_running || m_rep_running); }
        );
    }

    assert(!m_req_running && !m_rep_running);
    return !(m_req_running || m_rep_running);
}


inline
Process_message_reader::~Process_message_reader()
{
    if (!stop_and_wait(22.)) {
        // 'wait_for' timed out. To avoid hanging, we exit.
        // If we get here, something must have probably gone wrong.
        // Weird things might happen while exiting.
        exit(1);
    }

    delete m_request_reader_thread;
    delete m_in_req_c;
    delete m_reply_reader_thread;
    delete m_in_rep_c;
}


// This implementation of the following functions assumes the following:
// We have two types of messages: rpc messages and events
// - RPC messages are only addressed to specific receivers. All receivers able to handle this
//   type of message use the same handler, thus the handler map is static.
// - Events are only addressed "to whom it may concern" (i.e. any_*).
//   Their handlers are registered at process level, but they are assigned dynamically, thus
//   they may differ across different instances of the same type of receiver type.


inline
void Process_message_reader::request_reader_function()
{
    install_signal_handler();

    tl_is_req_thread = true;

    s_mproc->m_num_active_readers_mutex.lock();
    s_mproc->m_num_active_readers++;
    s_mproc->m_num_active_readers_mutex.unlock();

    m_in_req_c->start_reading();
    m_req_running = true;

    while (m_reader_state != READER_STOPPING) {
        s_tl_current_message = nullptr;

        // if there is an interprocess barrier and m_in_req_c has reached the barrier's sequence,
        // then the barrier is good to go.
        if (!s_mproc->m_flush_sequence.empty()) {
            auto reading_sequence = m_in_req_c->get_message_reading_sequence();
            while (reading_sequence >= s_mproc->m_flush_sequence.front()) {
                lock_guard<mutex> lk(s_mproc->m_flush_sequence_mutex);
                s_mproc->m_flush_sequence.pop_front();
                s_mproc->m_flush_sequence_condition.notify_one();
                if (s_mproc->m_flush_sequence.empty()) {
                    break;
                }
            }
        }

        Message_prefix* m = m_in_req_c->fetch_message();
        s_tl_current_message = m;
        if (m == nullptr) {
            break;
        }

        // Only the process with the coordinator's instance is allowed to send messages on
        // someone else's behalf (for relay purposes).
        // TODO: If some process not being part of the core set of processes sends nonsense,
        // it might be a good idea to kill it. If it is in the core set of processes,
        // then it would be a bug.
        assert(m_in_req_c->m_id == process_of(m->sender_instance_id) ||
               m_in_req_c->m_id == process_of(s_coord_id));

        assert(m->message_type_id != not_defined_type_id);

        if (is_local_instance(m->receiver_instance_id)) {

            // If addressed to a specified local receiver, this may only be an RPC call,
            // thus the receiver must exist.
            assert(
                m_reader_state == READER_NORMAL ?
                    s_mproc->m_local_pointer_of_instance_id.find(m->receiver_instance_id) !=
                    s_mproc->m_local_pointer_of_instance_id.end()
                :
                    true
            );

            if ((m_reader_state == READER_NORMAL) ||
                (is_service_instance(m->receiver_instance_id) && s_coord) ||
                (m->sender_instance_id == s_coord_id) )
            {
                // If addressed to a specified local receiver, this may only be an RPC call,
                // thus the named receiver must exist.

                // if the receiver  registered handler, call the handler
                auto it = Transceiver::get_rpc_handler_map().find(m->message_type_id);
                assert(it != Transceiver::get_rpc_handler_map().end()); // this would be a library error
                (*it->second)(*m); // call the handler
            }
        }
        else
        if (m->receiver_instance_id >= any_remote) {

            // this is an interprocess event message.

            if ((m_reader_state == READER_NORMAL) ||
                (s_coord && m->message_type_id > (type_id_type)detail::reserved_id::base_of_messages_handled_by_coordinator))
            {
                lock_guard<recursive_mutex> sl(s_mproc->m_handlers_mutex);
                    
                // [ NEW IMPLEMENTATION - NOT COVERED ]
                // find handlers that operate with this type of message in this process

                auto& active_handlers = s_mproc->m_active_handlers;
                auto it_mt = active_handlers.find(m->message_type_id);

                if (it_mt != active_handlers.end()) {

                    instance_id_type sids[] = {
                        m->sender_instance_id,
                        any_remote,
                        any_local_or_remote
                    };

                    for (auto sid : sids) {
                        auto shl = it_mt->second.find(sid);
                        if (shl != it_mt->second.end()) {
                            for (auto& e : shl->second) {
                                e(*m);
                            }
                        }
                    }
                }
            }

            // if the coordinator is in this process, relay
            if (s_coord && !has_same_mapping(*m_in_req_c, *s_mproc->m_out_req_c)) {
                s_mproc->m_out_req_c->relay(*m);
            }
        }
        else {
            // a local event has no place in interprocess messages
            // this would be a bug.
            assert(m->receiver_instance_id != any_local);

            // a specific non-local receiver means an rpc to another process.
            // if the coordinator is in this process, relay
            if (s_coord && !has_same_mapping(*m_in_req_c, *s_mproc->m_out_req_c)) {
                // the message type is specified, thus it is a request
                s_mproc->m_out_req_c->relay(*m);
            }
        }

        if (tl_post_handler_function) {
            (*tl_post_handler_function)();
            delete tl_post_handler_function;
            tl_post_handler_function = nullptr;
        }
    }

    m_in_req_c->done_reading();

    s_mproc->m_num_active_readers_mutex.lock();
    s_mproc->m_num_active_readers--;
    s_mproc->m_num_active_readers_mutex.unlock();
    s_mproc->m_num_active_readers_condition.notify_all();

    std::lock_guard<std::mutex> lk(m_stop_mutex);
    m_req_running = false;
    m_stop_condition.notify_one();

    // could not use unique_ptr or a static std::function, due to the funny behaviour
    // of mingw - even if the function object was destroyed here explicitly.
    if (tl_post_handler_function) {
        delete tl_post_handler_function;
        tl_post_handler_function = nullptr;
    }
}



inline
void Process_message_reader::reply_reader_function()
{
    install_signal_handler();

    s_mproc->m_num_active_readers_mutex.lock();
    s_mproc->m_num_active_readers++;
    s_mproc->m_num_active_readers_mutex.unlock();

    m_in_rep_c->start_reading();
    m_rep_running = true;

    while (m_reader_state != READER_STOPPING) {
        s_tl_current_message = nullptr;
        Message_prefix* m = m_in_rep_c->fetch_message();
        s_tl_current_message = m;

        if (m == nullptr) {
            break;
        }

        // Only the process with the coordinator's instance is allowed to send messages on
        // someone else's behalf (for relay purposes).
        assert(m_in_rep_c->m_id == process_of(m->sender_instance_id) ||
               m_in_rep_c->m_id == process_of(s_coord_id));

        assert(m->receiver_instance_id != any_local);

        if (is_local_instance(m->receiver_instance_id)) {

            if ((m_reader_state == READER_NORMAL) ||
                (m->receiver_instance_id == s_coord_id && s_coord) ||
                (m->sender_instance_id   == s_coord_id) )
            {
                auto it = s_mproc->m_local_pointer_of_instance_id.find(m->receiver_instance_id);

                if (it != s_mproc->m_local_pointer_of_instance_id.end()) {
                    auto &return_handlers = it->second->m_active_return_handlers;

                    it->second->m_return_handlers_mutex.lock();
                    auto it2 = return_handlers.find(m->function_instance_id);
                    it->second->m_return_handlers_mutex.unlock();

                    if (it2 != return_handlers.end()) {
                        if (m->exception_type_id == not_defined_type_id) {
                            it2->second.return_handler(*m);
                        }
                        else
                        if (m->exception_type_id != (type_id_type)detail::reserved_id::deferral) {
                            it2->second.exception_handler(*m);
                        }
                        else {
                            it2->second.deferral_handler(*m);
                        }
                    }
                    else {
                        // If it exists, there must be a return handler assigned.
                        // This is most likely an error local to this process.
                        assert(!"There is no active handler for the function return.");
                    }
                }
                else {
                    // This can occur by both local and remote error.
                    assert(!"The object that this return message refers to does not exist.");
                }
            }
        }
        else {

            // A specific non-local receiver implies an rpc call to another process,
            // thus if the coordinator is in the current process, relay -
            // unless the message originates from the ring we would relay to.
            if (s_coord && !has_same_mapping(*s_mproc->m_out_rep_c, *m_in_rep_c) ) {
                // the message type is not specified, thus it is a reply
                s_mproc->m_out_rep_c->relay(*m);
            }
        }
    }

    m_in_rep_c->done_reading();

    s_mproc->m_num_active_readers_mutex.lock();
    s_mproc->m_num_active_readers--;
    s_mproc->m_num_active_readers_mutex.unlock();
    s_mproc->m_num_active_readers_condition.notify_all();

    std::lock_guard<std::mutex> lk(m_stop_mutex);
    m_rep_running = false;
    m_stop_condition.notify_one();
}


} // namespace sintra

#endif
