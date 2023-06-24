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

#ifndef SINTRA_IPC_RINGS_H
#define SINTRA_IPC_RINGS_H

#include "config.h"

#include <atomic>
#include <cstdint>
#include <functional>
#include <iomanip>
#include <limits>
#include <string>
#include <thread>
#include <filesystem>
#include "get_wtime.h"

#include <boost/interprocess/detail/os_file_functions.hpp>
#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/sync/interprocess_mutex.hpp>
#include <boost/interprocess/sync/interprocess_semaphore.hpp>

#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>
#endif


#include "id_types.h"


namespace sintra {


using std::atomic;
using std::atomic_flag;
using std::error_code;
using std::string;
using std::stringstream;
using std::thread;
namespace fs  = std::filesystem;
namespace ipc = boost::interprocess;



/*

This is an implementation of an interprocess circular ring buffer,
accessed by a single writer and multiple concurrent readers.

Concepts
--------
1. The circular ring buffer consists of a data segment, and a control data segment.
   Both of them have a filesystem mapping.

2. Readers and writers are implemented with separate types, which derive from a basic
   'Ring' type, and provide accessors accordingly.

3. Conceptually the writer may only write immediately after an always increasing
   'leading sequence', thus strictly acting as 'producer'.

4. Readers may follow the writer acting as typical 'consumers', but they may as
   well access a contiguous part of the ringbuffer trailing the "leading sequence".
   The size of this trailing part is constant throughout the lifetime of the reader
   and must not exceed 3/4 of the buffer's size.

5. The buffer's data region is mapped to virtual memory twice consecutively, to
   allow the buffer's wrapping being handled natively. This is an optimization
   technique which is sometimes referred to as "the magic ring buffer".

6. A conceptual segmentation of the buffer's region into 8 octiles, allows
   handling the data structure atomically, and in a cache-friendly way.
   A shared atomically accessed 64-bit integer is used as a control variable, to
   communicate the number of readers per octile, on each of its 8 bytes. The variable
   is always handled atomically and on its whole, thus this is a SWAR technique.
   Before a write operation, the writer checks if the operation is on the same octile
   as the previous write operation, and only if it is not, it will have to check if
   the octile intended to be written is being read. If it is, the writer will block.
   An advantage of this method is that, unless there is contention, the writer will
   not even have to access the shared control variable more than 8 times in a full loop.
   That is because once the writer has reached a certain octile, it is guaranteed
   that there will be no data races in this octile.

Limitations
-----------
1. The aforementioned configuration, limits the number of readers to a maximum of
   255. This limit could easily be increased to 16383, if the ring was partitioned
   in quarters, rather than octiles. In this case though, the maximum trailing segment
   of a reader would not exceed 1/2 of the size of the ringbuffer, which would be
   a substantial tradeoff to memory efficiency.

2. For code simplicity, the writer may not write more data than the size of an octile in
   a single operation. This limitation is not absolutely necessary, but removing it
   would require some corner case handling, with a minor performance hit.


Remarks
-------
1. Sequence counters are of type uint64_t and count indefinitely, but overflowing a
   uint64_t is practically not going to be an issue during the lifetime of this code.

2. The default reading policy is SINTRA_RING_READING_POLICY_HYBRID, which causes the
   reader to spin for a specified period of time, before it goes to sleep, waiting on
   a semaphore. This policy was found to work adequately well in most cases.
   Nevertheless, performance of reading policies is not portable. It depends on
   usage, hardware and OS.

3. The choice of omp_get_wtime() for timing is because it was measured to work at least
   2x faster than comparable functions from std::chrono. This might not be the case
   across different systems.

*/


using sequence_counter_type = uint64_t;
constexpr auto invalid_sequence = ~sequence_counter_type(0);

// helpers
inline
bool check_or_create_directory(const string& dir_name)
{
    bool c = true;
    error_code ec;
    fs::path ps(dir_name);
    if (!fs::exists(ps)) {
        c &= fs::create_directory(ps, ec);
    }
    else
    if (fs::is_regular_file(ps)) {
        c &= fs::remove(ps, ec);
        c &= fs::create_directory(ps, ec);
    }
    return c;
}



inline
bool remove_directory(const string& dir_name)
{
    error_code ec;
    fs::path ps(dir_name);
    auto rv = fs::remove_all(dir_name.c_str(), ec);

    return !!rv;
}



template <typename T>
std::vector<size_t> get_ring_configurations(
    size_t min_elements, size_t max_size, size_t max_subdivisions)
{
    auto gcd = [](size_t m, size_t n)
    {
        size_t tmp;
        while(m) { tmp = m; m = n % m; n = tmp; }
        return n;
    };

    auto lcm = [=](size_t m, size_t n)
    {
        return m / gcd(m, n) * n;
    };

    size_t page_size = boost::interprocess::mapped_region::get_page_size();
    size_t base_size = lcm(sizeof(T), page_size);
    size_t min_size = std::max(min_elements * sizeof(T), base_size);
    size_t tmp_size = base_size;

    std::vector<size_t> ret;

    while (tmp_size*2 <= max_size) {
        tmp_size *= 2;
    }

    for (size_t i=0; i<max_subdivisions && tmp_size >= min_size; i++) {
        ret.push_back(tmp_size/sizeof(T));
        tmp_size /= 2;
    }

    std::reverse(std::begin(ret), std::end(ret));

    return ret;
}



template <typename T>
struct Range
{
    T* begin = nullptr;
    T* end   = nullptr;
};



class ring_acquisition_failure_exception {};



// A binary semaphore, which is only meant to be used in the context of
// this file, with semantics relevant to the ring's synchronization algorithm.
struct sintra_ring_semaphore : ipc::interprocess_semaphore
{
    sintra_ring_semaphore() : ipc::interprocess_semaphore(0) {}

    // This post is used when all waiting readers are woken up,
    // i.e. when writing new data, or unblocking globally.
    void post_ordered()
    {
        if (unordered.load()) { // post_unordered() was already called
            unordered = false;
        }
        else
        if (!posted.test_and_set(std::memory_order_acquire)) { // if not posted
            post();
        }
    }

    // This is used in operations where only a single reader is woken up,
    // while the rest must be left in their current state.
    void post_unordered()
    {
        if (!posted.test_and_set(std::memory_order_acquire)) { // if not posted
            post();
            unordered = true;
        }
    }

    // Regular semaphore wait(), returning true if the post responsible for the wakeup
    // was unordered and there was no call to post_ordered in-between.
    bool wait()
    {
        ipc::interprocess_semaphore::wait();
        posted.clear(std::memory_order_release);
        return unordered.load();
    }
private:
    atomic_flag     posted      = ATOMIC_FLAG_INIT;
    atomic<bool>    unordered   = false;
};



 //////////////////////////////////////////////////////////////////////////
///// BEGIN Ring_data //////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
//////   \//////   \//////   \//////   \//////   \//////   \//////   \//////
 ////     \////     \////     \////     \////     \////     \////     \////
  //       \//       \//       \//       \//       \//       \//       \//



// Ring sizes should be powers of 2 and multiple of the page size
template <typename T, bool READ_ONLY_DATA>
struct Ring_data
{
    using                   element_t       = T;
    constexpr static bool   read_only_data  = READ_ONLY_DATA;


    Ring_data(
        const string& directory,
        const string& data_filename,
        const size_t num_elements)
    :
        m_num_elements(num_elements),
        m_data_region_size(num_elements * sizeof(T))
    {
        m_directory = directory + "/";
        m_data_filename = m_directory + data_filename;

        fs::path pr(m_data_filename);

        bool c1 = fs::exists(pr) && fs::is_regular_file(pr) && fs::file_size(pr);
        bool c2 = c1 || create();

        if (!c2 || !attach()) {
            throw ring_acquisition_failure_exception();
        }

        std::hash<string> hasher;
        m_data_filename_hash = hasher(m_data_filename);
    }


    ~Ring_data()
    {
        delete m_data_region_0;
        delete m_data_region_1;

        m_data_region_0 = nullptr;
        m_data_region_1 = nullptr;
        m_data = nullptr;

        if (m_remove_files_on_destruction) {
            error_code ec;
            remove(fs::path(m_data_filename), ec);
        }
    }

    size_t   get_num_elements() const { return m_num_elements; }
    const T* get_base_address() const { return m_data;         }

private:
    using region_ptr_type = ipc::mapped_region*;

    bool create()
    {
        try {
            if (!check_or_create_directory(m_directory))
                return false;

            ipc::file_handle_t fh_data =
                ipc::ipcdetail::create_new_file(m_data_filename.c_str(), ipc::read_write);
            if (fh_data == ipc::ipcdetail::invalid_file())
                return false;

#ifdef NDEBUG
            if (!ipc::ipcdetail::truncate_file(fh_data, m_data_region_size))
                return false;
#else
            auto ustring = "UNINITIALIZED";
            auto dv = strlen(ustring);
            char* u_data = new char[m_data_region_size];
            for (size_t i=0; i<m_data_region_size; i++)
                u_data[i] = ustring[i%dv];

            ipc::ipcdetail::write_file(fh_data,  u_data, m_data_region_size);

            delete [] u_data;
#endif
            return ipc::ipcdetail::close_file(fh_data);
        }
        catch (...) {
        }
        return false;
    }


    bool attach()
    {
        assert(
            m_data_region_0  == nullptr &&
            m_data_region_1  == nullptr &&
            m_data           == nullptr);

        try {
            if (fs::file_size(m_data_filename) != m_data_region_size) {
                return false;
            }

            // NOTE: just to be clear, on Windows, this function does not return the page size.
            // It returns the "allocation granularity" (see dwAllocationGranularity in SYSTEM_INFO),
            // which is usually hardcoded to 64KB and refers to VirtualAlloc's allocation granularity.
            // However, this is what we actually need here, the page size is not relevant.
            size_t page_size = ipc::mapped_region::get_page_size();
            assert(m_data_region_size % page_size == 0);

#ifdef _WIN32
            void* mem = VirtualAlloc(NULL, m_data_region_size * 2 + page_size, MEM_RESERVE, PAGE_READWRITE);
#else
            // WARNING: This might eventually require a system specific implementation.
            // [translation: it has not failed so far, thus it's still here. If it fails, reimplement]
            void* mem = malloc(m_data_region_size * 2 + page_size);
#endif
            if (!mem) {
                return false;
            }

            char *ptr = (char*)(ptrdiff_t((char *)mem + page_size) & ~(page_size - 1));

            auto data_rights = READ_ONLY_DATA ? ipc::read_only : ipc::read_write;
            ipc::file_mapping file(m_data_filename.c_str(), data_rights);

            ipc::map_options_t map_extra_options = 0;

#ifdef _WIN32
            // here we make the assumption that between the release and the mapping that follows afterwards,
            // there will not be an allocation from a different thread, that could mess it all up.
            VirtualFree(mem, 0,  MEM_RELEASE);

#else
            // on Linux however, we do not free.
    #ifdef MAP_FIXED
            map_extra_options |= MAP_FIXED;
    #endif

    #ifdef MAP_NOSYNC
            map_extra_options |= MAP_NOSYNC
    #endif
#endif

            m_data_region_0 = new ipc::mapped_region(
                file, data_rights, 0, m_data_region_size, ptr, map_extra_options);
            m_data_region_1 = new ipc::mapped_region(
                file, data_rights, 0, 0,
                ((char*)m_data_region_0->get_address()) + m_data_region_size, map_extra_options);
            m_data = (T*)m_data_region_0->get_address();

            assert(m_data_region_0->get_address() == ptr);
            assert(m_data_region_1->get_address() == ptr + m_data_region_size);
            assert(m_data_region_0->get_size() == m_data_region_size);
            assert(m_data_region_1->get_size() == m_data_region_size);

            return true;
        }
        catch (...) {
            return false;
        }
    }


    region_ptr_type                     m_data_region_0                 = nullptr;
    region_ptr_type                     m_data_region_1                 = nullptr;
    string                              m_directory;

protected:

    const size_t                        m_num_elements;
    const size_t                        m_data_region_size;

    T*                                  m_data                          = nullptr;
    string                              m_data_filename;
    size_t                              m_data_filename_hash            = 0;
    bool                                m_remove_files_on_destruction   = false;

    template <typename RingT1, typename RingT2>
    friend bool has_same_mapping(const RingT1& r1, const RingT2& r2);
};


template <typename RingT1, typename RingT2>
bool has_same_mapping(const RingT1& r1, const RingT2& r2)
{
    return r1.m_data_filename_hash == r2.m_data_filename_hash;
}



  //\       //\       //\       //\       //\       //\       //\       //
 ////\     ////\     ////\     ////\     ////\     ////\     ////\     ////
//////\   //////\   //////\   //////\   //////\   //////\   //////\   //////
////////////////////////////////////////////////////////////////////////////
///// END Ring_data ////////////////////////////////////////////////////////
 //////////////////////////////////////////////////////////////////////////


 //////////////////////////////////////////////////////////////////////////
///// BEGIN Ring ///////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
//////   \//////   \//////   \//////   \//////   \//////   \//////   \//////
 ////     \////     \////     \////     \////     \////     \////     \////
  //       \//       \//       \//       \//       \//       \//       \//



template <typename T>
struct cache_line_sized_t
{
    T       v;
    uint8_t padding[assumed_cache_line_size - sizeof(T)];
};




template <typename T, bool READ_ONLY_DATA>
struct Ring: Ring_data<T, READ_ONLY_DATA>
{

    struct Control
    {
        // This struct is always instantiated in a memory region which is shared among processes.
        // If my understanding is correct, and the implementation is also correct, the use of
        // atomics should be fine for that purpose. [see N3337 29.4]

        atomic<size_t>                  num_attached = 0;

        // The index of the nth element written to the ringbuffer.
        atomic<sequence_counter_type>   leading_sequence = 0;

        // An 8-byte integer, with each byte corresponding to an octile of the ring
        // and representing the number of readers currently accessing it.
        // This imposes a limit of maximum 255 readers per ring.
        atomic<uint64_t>                read_access = 0;



        // NOTE: ONLY RELEVANT FOR TRACKING PURPOSES
        // It should always be equal to the sum of the value of the bytes of read_access.
        atomic<uint32_t>                num_readers = 0;

        cache_line_sized_t<sequence_counter_type>
                                        reading_sequences[max_process_index];


        int                             free_rs_stack[max_process_index];
        int                             num_free_rs = max_process_index;


        // Used to avoid accidentally having multiple writers on the same ring
        ipc::interprocess_mutex         ownership_mutex;

#if SINTRA_RING_READING_POLICY != SINTRA_RING_READING_POLICY_ALWAYS_SPIN

        // The following synchronization structures may only be accessed between lock()/unlock().

        // An array (pool) of semaphores which may be used to synchronize writing operations.
        sintra_ring_semaphore           dirty_semaphores[max_process_index];

        // A stack of indices to the dirty_semaphores array, with the semaphores which are
        // free and ready for use. Initially all semaphores are ready.
        int                             ready_stack[max_process_index];
        int                             num_ready = max_process_index;

        // A stack of indices to the dirty_semaphores array, with semaphores which have been
        // allocated to a reader from the ready_stack, and are either blocking, or are about
        // to block, or were previously blocking but have not been moved to another stack yet.
        int                             sleeping_stack[max_process_index];
        int                             num_sleeping = 0;

        // A stack of indices to the dirty_semaphores array, with semaphores which have been
        // posted out of order, i.e. after unblocking a reader locally.
        // To avoid a costly operation of moving a random element out of the stack, when a
        // semaphore is posted out-of-order, its index remains in the sleeping_stack, yet
        // the semaphore itself is flagged to avoid being reposted. Once an in-order post()
        // operation occurs, where all semaphores are posted (e.g. when writing), these indices
        // will be unflagged and placed back to the ready_stack.
        int                             unordered_stack[max_process_index];
        int                             num_unordered = 0;

        atomic_flag                     spinlock_flag = ATOMIC_FLAG_INIT;
#endif


        Control()
        {
#if SINTRA_RING_READING_POLICY != SINTRA_RING_READING_POLICY_ALWAYS_SPIN
            for (int i = 0; i < max_process_index; i++) { ready_stack   [i]  =  i; }
            for (int i = 0; i < max_process_index; i++) { sleeping_stack[i]  = -1; }
            for (int i = 0; i < max_process_index; i++) { unordered_stack[i] = -1; }
#endif

            for (int i = 0; i < max_process_index; i++) { reading_sequences[i].v = invalid_sequence; }
            for (int i = 0; i < max_process_index; i++) { free_rs_stack[i] = i; }


            // See the 'Note' in N4713 32.5 [Lock-free property], Par. 4.
            // The program is only valid if the conditions below are true.
            
            // On a second read... quoting the aforementioned note from the draft:
            //
            // "Operations that are lock-free should also be address-free. That is, atomic operations on the
            // same memory location via two different addresses will communicate atomically. The implementation
            // should not depend on any per-process state. This restriction enables communication by memory
            // that is mapped into a process more than once and by memory that is shared between two processes."
            //
            // ...so if they are not lock-free, could it be that they are not address-free?
            // My guess is that the author's intention was to stress that atomic operations are address-free
            // either way. Nevertheless, these assertions are just to be on the safe side.

            assert(num_attached.is_lock_free());        // size_t
            assert(leading_sequence.is_lock_free());    // sequence_counter_type
        }


#if SINTRA_RING_READING_POLICY != SINTRA_RING_READING_POLICY_ALWAYS_SPIN
        void lock()   { while (spinlock_flag.test_and_set(std::memory_order_acquire)) {} }
        void unlock() { spinlock_flag.clear(std::memory_order_release); }
#endif
    };



    Ring(const string& directory, const string& data_filename, size_t num_elements)
    :
        Ring_data<T, READ_ONLY_DATA>(directory, data_filename, num_elements)
    {
        m_control_filename = this->m_data_filename + "_control";

        fs::path pc(m_control_filename);

        bool c1 = fs::exists(pc) && fs::is_regular_file(pc) && fs::file_size(pc);
        bool c2 = c1 || create();

        if (!c2 || !attach()) {
            throw ring_acquisition_failure_exception();
        }

        if (!c1) {
            try {
                new (m_control) Control;
            }
            catch (...) {
                throw ring_acquisition_failure_exception();
            }
        }

        m_control->num_attached++;
    }


    ~Ring()
    {
        if (m_control->num_attached-- == 1) {
            this->m_remove_files_on_destruction = true;
        }

        delete m_control_region;
        m_control_region = nullptr;

        if (this->m_remove_files_on_destruction) {
            error_code ec;
            remove(fs::path(m_control_filename), ec);
        }
    }


    sequence_counter_type get_leading_sequence() const { return m_control->leading_sequence.load(); }


    T* get_element_from_sequence(sequence_counter_type sequence) const
    {
        auto leading_sequence = m_control->leading_sequence.load();
        if (sequence < this->m_num_elements) {
            return this->m_data + sequence;
        }
        auto first_sequence = leading_sequence - this->m_num_elements - 1;
        if (sequence < first_sequence || sequence > leading_sequence) {
            return nullptr;
        }

        return this->m_data + std::max(int64_t(0), int64_t(sequence)) % this->m_num_elements;
    }

private:
    using region_ptr_type = ipc::mapped_region*;

    bool create()
    {
        try {
            ipc::file_handle_t fh_control =
                ipc::ipcdetail::create_new_file(m_control_filename.c_str(), ipc::read_write);
            if (fh_control == ipc::ipcdetail::invalid_file())
                return false;

#ifdef NDEBUG
            if (!ipc::ipcdetail::truncate_file(fh_control, sizeof(Control)))
                return false;
#else
            auto ustring = "UNINITIALIZED";
            auto dv = strlen(ustring);
            char* u_control = new char[sizeof(Control)];
            for (size_t i=0; i<sizeof(Control); i++)
                u_control[i] = ustring[i%dv];

            ipc::ipcdetail::write_file(fh_control, u_control,sizeof(Control));

            delete [] u_control;
#endif
            return ipc::ipcdetail::close_file(fh_control);
        }
        catch (...) {
        }
        return false;
    }


    bool attach()
    {
        assert(m_control_region == nullptr);

        try {
            if (fs::file_size(m_control_filename) != sizeof(Control)) {
                return false;
            }

            ipc::file_mapping fm_control(m_control_filename.c_str(), ipc::read_write);
            m_control_region = new ipc::mapped_region(fm_control, ipc::read_write, 0, 0);
            m_control = (Control*)m_control_region->get_address();

            return true;
        }
        catch (...) {
            return false;
        }
    }


private:
    region_ptr_type                     m_control_region    = nullptr;
    string                              m_control_filename;
    
protected:
    Control*                            m_control           = nullptr;
};





  //\       //\       //\       //\       //\       //\       //\       //
 ////\     ////\     ////\     ////\     ////\     ////\     ////\     ////
//////\   //////\   //////\   //////\   //////\   //////\   //////\   //////
////////////////////////////////////////////////////////////////////////////
///// END Ring /////////////////////////////////////////////////////////////
 //////////////////////////////////////////////////////////////////////////


 //////////////////////////////////////////////////////////////////////////
///// BEGIN Ring_R /////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
//////   \//////   \//////   \//////   \//////   \//////   \//////   \//////
 ////     \////     \////     \////     \////     \////     \////     \////
  //       \//       \//       \//       \//       \//       \//       \//



// To have this ring work as consumer, the sequence is
// start_reading
//     wait_for_new_data
//     done_reading_new_data
// done_reading



template <typename T>
struct Ring_R: Ring<T, true>
{
    Ring_R(const string& directory, const string& data_filename,
        size_t num_elements, size_t max_trailing_elements = 0)
    :
        Ring<T, true>::Ring(directory, data_filename, num_elements)
    ,   m_max_trailing_elements(max_trailing_elements)
    ,   c(*this->m_control)
    {
        // num_elements must be a multiple of 8
        assert(num_elements % 8 == 0);

        // m_max_trailing_elements may not be grater than 3/4 of the size of the ring
        assert(max_trailing_elements <= 3 * num_elements / 4);
    }


    ~Ring_R()
    {
        done_reading();
    }


    Range<T> start_reading()
    {
        return start_reading(m_max_trailing_elements);
    }


    // Start reading up to num_trailing_elements elements behind the leading sequence.
    // When data is no longer being accessed, the caller should call done_reading(),
    // to prevent blocking the writer.
    Range<T> start_reading(size_t num_trailing_elements)
    {
        bool f = false;
        while (!m_reading_lock.compare_exchange_strong(f, true)) { f = false; }

        m_reading = true;

        assert(num_trailing_elements <= m_max_trailing_elements);

        // this prevents the writer from progressing beyond the end of the octile that
        // succeeds the one it is currently on
        uint64_t access_mask = 0x0101010101010101;
        c.read_access += access_mask;

        // reading out the leading sequence atomically, ensures that the return range
        // will not exceed num_trailing_elements
        auto leading_sequence = c.leading_sequence.load();

        auto range_first_sequence =
            std::max(int64_t(0), int64_t(leading_sequence) - int64_t(num_trailing_elements));

        Range<T> ret;
        ret.begin = this->m_data +
            std::max(int64_t(0), int64_t(range_first_sequence)) % this->m_num_elements;
        ret.end   = ret.begin + leading_sequence - range_first_sequence;

        assert(ret.end >= ret.begin);
        assert(ret.begin >= this->m_data);

        m_trailing_octile =
            (8 * ((range_first_sequence - m_max_trailing_elements) % this->m_num_elements)) /
            this->m_num_elements;

        access_mask   -= uint64_t(1) << (8 * m_trailing_octile);
        c.read_access -= access_mask;

        // allocate reading sequence
        m_rs_index = c.free_rs_stack[--c.num_free_rs];
        m_reading_sequence = &c.reading_sequences[m_rs_index].v;

        // advance to the leading sequence that was read in the beginning
        // this way the function will work orthogonally to wait_for_new_data()
        *m_reading_sequence = leading_sequence;

        m_reading_lock = false;
        return ret;
    }


    // this reading ring will no longer block the writer
    void done_reading()
    {
        bool f = false;
        while (!m_reading_lock.compare_exchange_strong(f, true)) { f = false; }
        if (m_reading) {
            c.read_access -= uint64_t(1) << (8 * m_trailing_octile);
            *m_reading_sequence = m_trailing_octile = 0;
            m_reading = false;

            // release reading sequence
            c.free_rs_stack[c.num_free_rs++] = m_rs_index;
            m_rs_index = -1;
            m_reading_sequence = &s_zero_rs;
        }
        m_reading_lock = false;
    }

    sequence_counter_type reading_sequence() const { return *m_reading_sequence; }

    // Returns a range with the elements succeeding the current reader's reading sequence,
    // and sets the reading sequence to the current leading sequence
    // The function will block until elements become available or it is explicitly unblocked.
    // The caller must call done_reading_new_data() to move the read
    const Range<T> wait_for_new_data()
    {

#if SINTRA_RING_READING_POLICY == SINTRA_RING_READING_POLICY_ALWAYS_SPIN

        while (*m_reading_sequence == c.leading_sequence.load() ) {}

#else // SINTRA_RING_READING_POLICY_HYBRID or SINTRA_RING_READING_POLICY_ALWAYS_SLEEP

#if SINTRA_RING_READING_POLICY == SINTRA_RING_READING_POLICY_HYBRID

        double tl = get_wtime() + spin_before_sleep * 0.5;
        while (*m_reading_sequence == c.leading_sequence.load() && get_wtime() < tl) {}

#endif

        c.lock();
        m_sleepy_index = -1;
        if (*m_reading_sequence == c.leading_sequence.load()) {
            m_sleepy_index = c.ready_stack[--c.num_ready];
            c.sleeping_stack[c.num_sleeping++] = m_sleepy_index;
        }
        c.unlock();

        if (m_sleepy_index >= 0) {
            if (c.dirty_semaphores[m_sleepy_index].wait()) { // unordered
                c.lock();
                c.unordered_stack[c.num_unordered++] = m_sleepy_index;
            }
            else { // ordered
                c.lock();
                c.ready_stack[c.num_ready++] = m_sleepy_index;
            }
            m_sleepy_index = -1;
            c.unlock();
        }

#endif

        Range<T> ret;

        auto num_range_elements = size_t(c.leading_sequence.load() - *m_reading_sequence);

        if (num_range_elements == 0) {
            // this may happen if the loop was unblocked, either locally with unblock_local(),
            // ore remotely by the writer, with unblock_global();
            return ret;
        }

        ret.begin = this->m_data + (*m_reading_sequence % this->m_num_elements);
        ret.end   = ret.begin + num_range_elements;
        *m_reading_sequence += num_range_elements;
        return ret;
    }


    // release the ring's range that was blocked for reading the new data
    // (it might still be blocked by other readers).
    void done_reading_new_data()
    {
        size_t new_trailing_octile =
            (8 * ((*m_reading_sequence - m_max_trailing_elements) % this->m_num_elements)) /
            this->m_num_elements;

        if (new_trailing_octile != m_trailing_octile) {
            auto diff =
                (uint64_t(1) << (8 * new_trailing_octile)) -
                (uint64_t(1) << (8 *   m_trailing_octile));
            c.read_access += diff;
            m_trailing_octile = new_trailing_octile;
        }
    }


    // If wait_for_new_data() is either sleeping or spinning on a different thread,
    // this call will force it to return a nullptr and 0 elements.
    // Only the local reader instance will be affected.
    void unblock_local()
    {
#if SINTRA_RING_READING_POLICY != SINTRA_RING_READING_POLICY_ALWAYS_SPIN
        c.lock();
        if (m_sleepy_index >= 0) {
            c.dirty_semaphores[m_sleepy_index].post_unordered();
        }
        c.unlock();
#endif
    }


    sequence_counter_type get_reading_sequence() const { return *m_reading_sequence; }


protected:
    const size_t                        m_max_trailing_elements;
    sequence_counter_type*              m_reading_sequence          = &s_zero_rs;
    size_t                              m_trailing_octile           = 0;
    atomic<bool>                        m_reading = false;
    atomic<bool>                        m_reading_lock = false;
    int                                 m_sleepy_index              = -1;
    int                                 m_rs_index                  = -1;

    inline static sequence_counter_type s_zero_rs = 0;

    typename Ring<T, true>::Control&    c;
};



  //\       //\       //\       //\       //\       //\       //\       //
 ////\     ////\     ////\     ////\     ////\     ////\     ////\     ////
//////\   //////\   //////\   //////\   //////\   //////\   //////\   //////
////////////////////////////////////////////////////////////////////////////
///// END Ring_R ///////////////////////////////////////////////////////////
 //////////////////////////////////////////////////////////////////////////


 //////////////////////////////////////////////////////////////////////////
///// BEGIN Ring_W /////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
//////   \//////   \//////   \//////   \//////   \//////   \//////   \//////
 ////     \////     \////     \////     \////     \////     \////     \////
  //       \//       \//       \//       \//       \//       \//       \//


struct void_placeholder_t{};


template <typename T>
struct Ring_W: Ring<T, false>
{
    Ring_W(const string& directory, const string& data_filename, size_t num_elements):
        Ring<T, false>::Ring(directory, data_filename, num_elements),
        c(*this->m_control)
    {
        if (!c.ownership_mutex.try_lock()) {
            throw ring_acquisition_failure_exception();
        }
    }

    ~Ring_W()
    {
        unblock_global();
        c.ownership_mutex.unlock();
    }


    // The generic version, writing a buffer of ring elements
    T* write(const T* src_buffer, size_t num_src_elements)
    {
        T* write_location = prepare_write(num_src_elements);

        for (size_t i = 0; i < num_src_elements; i++) {
            write_location[i] = src_buffer[i];
        }
        return write_location;
    }


    // The specialized version, for constructing an arbitrary type object in-place into the ring
    // sizeof(T2) must be a multiple of sizeof(T)
    template <typename T2, typename... Args>
    T2* write(size_t num_extra_elements, Args... args)
    {
        assert(sizeof(T2) % sizeof(T) == 0);
        auto num_elements = sizeof(T2)/sizeof(T)+num_extra_elements;

        T2* write_location = (T2*) prepare_write(num_elements);

        // write (construct) the message
        return new (write_location) T2{args...};
    }

    sequence_counter_type done_writing()
    {
        // update sequence
        // after the next line, any comparison of the form:
        // m_reading_sequence == m_control->leading_sequence
        // on the reader will keep failing until done_reading() is called

#if SINTRA_RING_READING_POLICY != SINTRA_RING_READING_POLICY_ALWAYS_SPIN

        c.lock();
        assert(m_writing_thread == std::this_thread::get_id());

        // the next statement must be inside the lock, because leading_sequence
        // is also used to signal explicit unblocks. Otherwise a separate
        // variable would be required.
        c.leading_sequence.store(m_pending_new_sequence);

        for (int i = 0; i < c.num_sleeping; i++) {
            c.dirty_semaphores[c.sleeping_stack[i]].post_ordered();
        }
        c.num_sleeping = 0;

        while (c.num_unordered) {
            c.ready_stack[c.num_ready++] = c.unordered_stack[--c.num_unordered];
        }
        c.unlock();

#else
        c.leading_sequence.store(m_pending_new_sequence);
#endif

        m_writing_thread = thread::id();
        return m_pending_new_sequence;
    }



    // If wait_for_new_data() is either sleeping or spinning, it will force it
    // to return a nullptr and 0 elements. This will affect any process or thread
    // reading from this ring.
    void unblock_global()
    {
#if SINTRA_RING_READING_POLICY != SINTRA_RING_READING_POLICY_ALWAYS_SPIN
        c.lock();
        for (int i = 0; i < c.num_sleeping; i++) {
            c.dirty_semaphores[c.sleeping_stack[i]].post_ordered();
        }
        c.num_sleeping = 0;

        while (c.num_unordered) {   
            c.ready_stack[c.num_ready++] = c.unordered_stack[--c.num_unordered];
        }
        c.unlock();
#endif
    }


    // This function allows a writer to read the ring's readable data, without locking.
    Range<T> get_readable_range()
    {
        auto leading_sequence = c.leading_sequence.load();
        auto range_first_sequence =
            std::max(int64_t(0), int64_t(leading_sequence) - int64_t(3 * this->m_num_elements / 4));

        Range<T> ret;
        ret.begin = this->m_data +
            std::max(int64_t(0), int64_t(range_first_sequence)) % this->m_num_elements;
        ret.end = ret.begin + leading_sequence - range_first_sequence;

        return ret;
    }


private:

    inline
    T* prepare_write(size_t num_elements_to_write)
    {
        assert(num_elements_to_write <= this->m_num_elements/8);

        // assure exclusive write access
        while (m_writing_thread != std::this_thread::get_id()) {
            auto invalid_thread = thread::id();
            m_writing_thread.compare_exchange_strong(invalid_thread, std::this_thread::get_id());
        }

        size_t index = m_pending_new_sequence % this->m_num_elements;
        m_pending_new_sequence += num_elements_to_write;
        size_t new_octile = (8 * (m_pending_new_sequence % this->m_num_elements)) / this->m_num_elements;

        // if the writing range has not been acquired
        if (m_octile != new_octile) {
            auto range_mask = (uint64_t(0xff) << (8 * new_octile));

            // if anyone is reading the octile range of the write operation,
            // wait (spin) to prevent an overwrite
            while (c.read_access & range_mask) {}

            m_octile = new_octile;
        }
        return this->m_data+index;
    }


    atomic<thread::id>          m_writing_thread;
    size_t                      m_octile                    = 0;
    sequence_counter_type       m_pending_new_sequence      = 0;

    typename Ring<T, false>::Control& c;
};



  //\       //\       //\       //\       //\       //\       //\       //
 ////\     ////\     ////\     ////\     ////\     ////\     ////\     ////
//////\   //////\   //////\   //////\   //////\   //////\   //////\   //////
////////////////////////////////////////////////////////////////////////////
///// END Ring_W ///////////////////////////////////////////////////////////
 //////////////////////////////////////////////////////////////////////////


 //////////////////////////////////////////////////////////////////////////
///// BEGIN Local_Ring_W ///////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
//////   \//////   \//////   \//////   \//////   \//////   \//////   \//////
 ////     \////     \////     \////     \////     \////     \////     \////
  //       \//       \//       \//       \//       \//       \//       \//

template <typename T>
struct Local_Ring_W : Ring_W<T>
{
    using Ring_W<T>::Ring_W;
};


//\       //\       //\       //\       //\       //\       //\       //
////\     ////\     ////\     ////\     ////\     ////\     ////\     ////
//////\   //////\   //////\   //////\   //////\   //////\   //////\   //////
////////////////////////////////////////////////////////////////////////////
///// END Local_Ring_W /////////////////////////////////////////////////////
 //////////////////////////////////////////////////////////////////////////

} // namespace sintra

#endif
