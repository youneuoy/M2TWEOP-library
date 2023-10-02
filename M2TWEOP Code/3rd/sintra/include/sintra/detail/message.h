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

#ifndef SINTRA_MESSAGE_H
#define SINTRA_MESSAGE_H

#include "id_types.h"
#include "ipc_rings.h"
#include "utility.h"

#include <cstdint>
#include <sstream>
#include <type_traits>

#include <boost/fusion/container/vector.hpp>



namespace sintra {


using std::enable_if_t;
using std::is_base_of;
using std::is_convertible;
using std::is_pod;
using std::is_same;
using std::remove_reference;


constexpr uint64_t  message_magic        = 0xc18a1aca1ebac17a;
constexpr int       message_ring_size    = 0x200000;


 //////////////////////////////////////////////////////////////////////////
///// BEGIN VARIABLE BUFFER ////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
//////   \//////   \//////   \//////   \//////   \//////   \//////   \//////
 ////     \////     \////     \////     \////     \////     \////     \////
  //       \//       \//       \//       \//       \//       \//       \//


// This type only lives inside ring buffers.
struct variable_buffer
{
    // this is set in the constructor of the derived type
    size_t num_bytes = 0;

    // this is set by the message constructor
    size_t offset_in_bytes = 0;

    template <typename = void>
    struct Statics
    {
        thread_local static char*        tl_message_start_address;
        thread_local static uint32_t*    tl_pbytes_to_next_message;
    };
    using S = Statics<void>;

    bool empty() const { return num_bytes == 0; }

    variable_buffer() {}

    variable_buffer(const variable_buffer&) = default;
    variable_buffer& operator=(const variable_buffer&) = default;

    template <typename TC, typename T = typename TC::iterator::value_type>
    variable_buffer(const TC& container);
};


template <> thread_local char*      variable_buffer::S::tl_message_start_address    = nullptr;
template <> thread_local uint32_t*  variable_buffer::S::tl_pbytes_to_next_message   = nullptr;


template <typename T>
struct typed_variable_buffer: protected variable_buffer
{
public:
    typed_variable_buffer() {}
    typed_variable_buffer(const T& v): variable_buffer(v) {}
    template <typename CT = typename T::iterator::value_type>
    operator T() const
    {
        assert(offset_in_bytes);
        assert((num_bytes % sizeof(CT)) == 0);

        CT* typed_data = (CT*)((char*)this+offset_in_bytes);
        size_t num_elements = num_bytes / sizeof(CT);
        return T(typed_data, typed_data + num_elements);
    }
};


inline
size_t vb_size()
{
    return 0;
}


template <
    typename T,
    typename = enable_if_t<
        !is_convertible< typename remove_reference<T>::type, variable_buffer>::value
    >,
    typename... Args
>
size_t vb_size(const T& v, Args&&... args);


template <
    typename T, typename = void,
    typename = enable_if_t<
        is_convertible< typename remove_reference<T>::type, variable_buffer>::value
    >,
    typename... Args
>
size_t vb_size(const T& v, Args&&... args);


template <
    typename T,
    typename /*= typename enable_if_t<
        !is_convertible< typename remove_reference<T>::type, variable_buffer>::value
    > */,
    typename... Args
>
size_t vb_size(const T&, Args&&... args)
{
    auto ret = vb_size(args...);
    return ret;
}


template <
    typename T, typename /* = void*/,
    typename /* = typename enable_if_t<
        is_convertible< typename remove_reference<T>::type, variable_buffer>::value
    >*/,
    typename... Args
>
size_t vb_size(const T& v, Args&&... args)
{
    auto ret = v.size() * sizeof(typename T::iterator::value_type) + vb_size(args...);
    return ret;
}


using message_string = typed_variable_buffer<string>;


  //\       //\       //\       //\       //\       //\       //\       //
 ////\     ////\     ////\     ////\     ////\     ////\     ////\     ////
//////\   //////\   //////\   //////\   //////\   //////\   //////\   //////
////////////////////////////////////////////////////////////////////////////
///// END VARIABLE BUFFER //////////////////////////////////////////////////
 //////////////////////////////////////////////////////////////////////////


 //////////////////////////////////////////////////////////////////////////
///// BEGIN SERIALIZABLE TYPE //////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
//////   \//////   \//////   \//////   \//////   \//////   \//////   \//////
 ////     \////     \////     \////     \////     \////     \////     \////
  //       \//       \//       \//       \//       \//       \//       \//


class Sintra_message_element {};


class No
{
private:
    No(){}
    virtual int no()=0;
};

template <
    typename T,
    bool =
        is_pod<T>::value ||
        is_base_of<Sintra_message_element, T>::value,
    bool = is_convertible<T, variable_buffer>::value
>
struct transformer
{
    using type = int; //PORT - THIS LINE IS WRONG
};


template <typename T, bool IRRELEVANT>
struct transformer<T, true, IRRELEVANT>
{
    using type = T;
};


template <typename T>
struct transformer<T, false, true>
{
    using type = typed_variable_buffer<T>;
};


template <typename SEQ_T, int I, int J, typename... Args>
struct serializable_type_impl;


template <typename SEQ_T, int I, typename... Args>
struct serializable_type_impl<SEQ_T, I, I, Args...>
{
    using type = typename boost::fusion::template vector<Args...>;
    static constexpr bool has_variable_buffers = false;
};


template <typename SEQ_T, int I, int J, typename... Args>
struct serializable_type_impl
{
    using arg_type = typename boost::fusion::result_of::value_at_c<SEQ_T, I>::type;
    using transformed_type =
        typename transformer<typename remove_reference<arg_type>::type>::type;
    using aggregate_type =
        serializable_type_impl<SEQ_T, I + 1, J, Args..., transformed_type>;
    using type = typename aggregate_type::type;
    static constexpr bool has_variable_buffers = aggregate_type::has_variable_buffers ||
        !is_same<
            typename remove_reference<arg_type        >::type,
            typename remove_reference<transformed_type>::type
        >::value;
};


template <
    typename SEQ_T,
    typename AGGREGATE = serializable_type_impl<
        SEQ_T,
        0,
        boost::fusion::result_of::template size<SEQ_T>::value
    >,
    typename BASE = typename AGGREGATE::type
>
struct serializable_type: BASE
{
    using BASE::BASE;
    static constexpr bool has_variable_buffers = AGGREGATE::has_variable_buffers;
};


template<typename... Args>
constexpr bool args_require_varbuffer =
    serializable_type<typename boost::fusion::template vector<Args...> >::has_variable_buffers;


  //\       //\       //\       //\       //\       //\       //\       //
 ////\     ////\     ////\     ////\     ////\     ////\     ////\     ////
//////\   //////\   //////\   //////\   //////\   //////\   //////\   //////
////////////////////////////////////////////////////////////////////////////
///// END SERIALIZABLE TYPE ////////////////////////////////////////////////
 //////////////////////////////////////////////////////////////////////////


 //////////////////////////////////////////////////////////////////////////
///// BEGIN MESSAGE ////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
//////   \//////   \//////   \//////   \//////   \//////   \//////   \//////
 ////     \////     \////     \////     \////     \////     \////     \////
  //       \//       \//       \//       \//       \//       \//       \//


// The prefix is the first part of any message sent through a ring.
// It contains information necessary for the dispatcher to deliver messages, which may as well
// be used by the recipient.
struct Message_prefix
{
    // used by the serializer as a basic corruption check
    const uint64_t magic                    = message_magic;

    // used by the serializer, set in constructor
    uint32_t bytes_to_next_message          = 0;

    union {
        // This is a cross-process type id. It is set in message constructor.
        type_id_type message_type_id        = invalid_type_id;

        // Only relevant in RPC return messages.
        type_id_type exception_type_id;
    };

    // Only instantiated in RPC messages, to associate the return messages with the original.
    // When this is set, the message_type_id becomes irrelevant.
    instance_id_type function_instance_id   = invalid_instance_id;

    // used by the serializer, set in communicators
    instance_id_type sender_instance_id     = invalid_instance_id;

    // used by the serializer, set in communicators
    instance_id_type receiver_instance_id   = invalid_instance_id;
};

template <typename T>
sintra::type_id_type get_type_id();

// This type only lives inside ring buffers.

template <
    typename T,
    typename RT = void,
    type_id_type ID = 0,
    typename EXPORTER = void
>
struct Message: public Message_prefix, public T
{
    using body_type = T;
    using return_type = RT;
    using exporter = EXPORTER;


    static type_id_type id()
    {
        static type_id_type tid = ID ? make_type_id(ID) : get_type_id<Message>();
        return tid;
    }


    template<
        typename... Args, typename = void,
        typename = enable_if_t< !args_require_varbuffer<Args...> >
    >
    Message(Args&& ...args)
    {
        bytes_to_next_message = sizeof(Message);

        void* body_ptr = static_cast<body_type*>(this);
        new (body_ptr) body_type{args...};

        assert(bytes_to_next_message < (message_ring_size / 8));

        message_type_id = id();
    }


    template<
        typename... Args,
        typename = enable_if_t< args_require_varbuffer<Args...> >
    >
    Message(Args&& ...args)
    {
        bytes_to_next_message = sizeof(Message);

        variable_buffer::S::tl_message_start_address = (char*) this;
        variable_buffer::S::tl_pbytes_to_next_message = &bytes_to_next_message;

        void* body_ptr = static_cast<body_type*>(this);
        new (body_ptr) body_type{args...};

        variable_buffer::S::tl_message_start_address = nullptr;
        variable_buffer::S::tl_pbytes_to_next_message = nullptr;

        assert(bytes_to_next_message < (message_ring_size / 8));

        message_type_id = id();
    }


    ~Message()
    {
    }
};


  //\       //\       //\       //\       //\       //\       //\       //
 ////\     ////\     ////\     ////\     ////\     ////\     ////\     ////
//////\   //////\   //////\   //////\   //////\   //////\   //////\   //////
////////////////////////////////////////////////////////////////////////////
///// END MESSAGE //////////////////////////////////////////////////////////
 //////////////////////////////////////////////////////////////////////////


 //////////////////////////////////////////////////////////////////////////
///// BEGIN SIGNAL MACROS //////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
//////   \//////   \//////   \//////   \//////   \//////   \//////   \//////
 ////     \////     \////     \////     \////     \////     \////     \////
  //       \//       \//       \//       \//       \//       \//       \//


// [imak]: the "internal helpers" are due to a bug of MSVC's implementation of variadic macros.
// At the time of writing, MSVC2013 update 5 is still exhibiting the problem.
// I stole the original workaround from here:
// http://stackoverflow.com/questions/24836793/varargs-elem-macro-for-use-with-c/24837037#24837037


/* internal helpers */
#define VA_NARGS_GLUE_(x, y) x y
#define VA_NARGS_RETURN_COUNT_(                                         \
    _1_ , _2_ , _3_ , _4_ , _5_ , _6_ , _7_ , _8_,                      \
    _9_ ,_10_, _11_, _12_, _13_, _14_, _15_, _16_, count, ...) count
#define VA_NARGS_EXPAND_(args) VA_NARGS_RETURN_COUNT_ args
#define VA_NARGS_COUNT_MAX16_(...) VA_NARGS_EXPAND_((__VA_ARGS__,       \
    16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0))

#define VA_NARGS_OVERLOAD_MACRO2_(name, count) name##count
#define VA_NARGS_OVERLOAD_MACRO1_(name, count) VA_NARGS_OVERLOAD_MACRO2_(name, count)
#define VA_NARGS_OVERLOAD_MACRO_(name,  count) VA_NARGS_OVERLOAD_MACRO1_(name, count)

/* expose for re-use */
#define VA_NARGS_CALL_OVERLOAD(name, ...) \
    VA_NARGS_GLUE_(VA_NARGS_OVERLOAD_MACRO_(name, VA_NARGS_COUNT_MAX16_(__VA_ARGS__)),(__VA_ARGS__))

/* overloads */
#define VA_DEFINE_STRUCT_1( v) \
    struct v { };
#define VA_DEFINE_STRUCT_2( v,a) \
    struct v { a; };
#define VA_DEFINE_STRUCT_3( v,a,b) \
    struct v { a;b; };
#define VA_DEFINE_STRUCT_4( v,a,b,c) \
    struct v { a;b;c; };
#define VA_DEFINE_STRUCT_5( v,a,b,c,d) \
    struct v { a;b;c;d; };
#define VA_DEFINE_STRUCT_6( v,a,b,c,d,e) \
    struct v { a;b;c;d;e; };
#define VA_DEFINE_STRUCT_7( v,a,b,c,d,e,f) \
    struct v { a;b;c;d;e;f; };
#define VA_DEFINE_STRUCT_8( v,a,b,c,d,e,f,g) \
    struct v { a;b;c;d;e;f;g; };
#define VA_DEFINE_STRUCT_9( v,a,b,c,d,e,f,g,h) \
    struct v { a;b;c;d;e;f;g;h; };
#define VA_DEFINE_STRUCT_10(v,a,b,c,d,e,f,g,h,i) \
    struct v { a;b;c;d;e;f;g;h;i; };
#define VA_DEFINE_STRUCT_11(v,a,b,c,d,e,f,g,h,i,j) \
    struct v { a;b;c;d;e;f;g;h;i;j; };
#define VA_DEFINE_STRUCT_12(v,a,b,c,d,e,f,g,h,i,j,k) \
    struct v { a;b;c;d;e;f;g;h;i;j;k; };
#define VA_DEFINE_STRUCT_13(v,a,b,c,d,e,f,g,h,i,j,k,l) \
    struct v { a;b;c;d;e;f;g;h;i;j;k;l; };
#define VA_DEFINE_STRUCT_14(v,a,b,c,d,e,f,g,h,i,j,k,l,m) \
    struct v { a;b;c;d;e;f;g;h;i;j;k;l;m; };
#define VA_DEFINE_STRUCT_15(v,a,b,c,d,e,f,g,h,i,j,k,l,m,n) \
    struct v { a;b;c;d;e;f;g;h;i;j;k;l;m;n; };
#define VA_DEFINE_STRUCT_16(v,a,b,c,d,e,f,g,h,i,j,k,l,m,n,o) \
    struct v { a;b;c;d;e;f;g;h;i;j;k;l;m;n;o; };

/* reusable macro */
#define DEFINE_STRUCT_(...) VA_NARGS_CALL_OVERLOAD(VA_DEFINE_STRUCT_, __VA_ARGS__)



#define SINTRA_SIGNAL_BASE(name, idv, ...)                                              \
    void inheritance_assertion_##name() {                                               \
        static_assert(std::is_same_v<                                                   \
            std::remove_pointer_t<decltype(this)>,                                      \
            Transceiver_type>,                                                          \
            "This Transceiver is not derived correctly."                                \
        );                                                                              \
    }                                                                                   \
    DEFINE_STRUCT_(sm_body_type_##name, __VA_ARGS__)                                    \
    using name = sintra::Message<sm_body_type_##name, void, idv, Transceiver_type>;     \


#define SINTRA_SIGNAL(name, ...)                                                        \
    SINTRA_SIGNAL_BASE(name, sintra::invalid_type_id, __VA_ARGS__)

#define SINTRA_SIGNAL_EXPLICIT(name, ...)                                               \
    SINTRA_SIGNAL_BASE(name, (type_id_type)sintra::detail::reserved_id::name, __VA_ARGS__)



  //\       //\       //\       //\       //\       //\       //\       //
 ////\     ////\     ////\     ////\     ////\     ////\     ////\     ////
//////\   //////\   //////\   //////\   //////\   //////\   //////\   //////
////////////////////////////////////////////////////////////////////////////
///// END SIGNAL MACROS/////////////////////////////////////////////////////
 //////////////////////////////////////////////////////////////////////////


 //////////////////////////////////////////////////////////////////////////
///// BEGIN ENCLOSURE //////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
//////   \//////   \//////   \//////   \//////   \//////   \//////   \//////
 ////     \////     \////     \////     \////     \////     \////     \////
  //       \//       \//       \//       \//       \//       \//       \//


template <
    typename T,
    bool = is_convertible<T, variable_buffer>::value,
    bool = is_pod<T>::value
>
struct Enclosure
{
};


template<>
struct Enclosure<void, false, false>
{
    void get_value() const {}
};


template <typename T>
struct Enclosure<T, false, true>
{
    T get_value() const { return value; }
    T value;
};


template <typename T>
struct Enclosure<T, true, false>
{
    T get_value() const { return value; }
    typed_variable_buffer<typename remove_reference<T>::type> value;
};



template <
    typename T,
    bool C1 = is_convertible<T, variable_buffer>::value,
    bool C2 = is_pod<T>::value
>
struct Unserialized_Enclosure: Enclosure <T, C1, C2>
{
    Unserialized_Enclosure& operator=(const Enclosure<T, C1, C2>& rhs)
    {
        Enclosure<T, C1, C2>::operator=(rhs);
        return *this;
    }
};



template <typename T>
struct Unserialized_Enclosure<T, true, false>: Enclosure <T, true, false>
{
    Unserialized_Enclosure() {}

    Unserialized_Enclosure& operator=(const Enclosure<T, true, false>& rhs)
    {
        free_value = rhs.value;
        return *this;
    }

    typename remove_reference<T>::type free_value;

    T get_value() const { return free_value; }
};


  //\       //\       //\       //\       //\       //\       //\       //
 ////\     ////\     ////\     ////\     ////\     ////\     ////\     ////
//////\   //////\   //////\   //////\   //////\   //////\   //////\   //////
////////////////////////////////////////////////////////////////////////////
///// END ENCLOSURE ////////////////////////////////////////////////////////
 //////////////////////////////////////////////////////////////////////////


 //////////////////////////////////////////////////////////////////////////
///// BEGIN MESSAGE RINGS //////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
//////   \//////   \//////   \//////   \//////   \//////   \//////   \//////
 ////     \////     \////     \////     \////     \////     \////     \////
  //       \//       \//       \//       \//       \//       \//       \//


inline
std::string get_base_filename(const string& prefix, uint64_t id)
{
    std::stringstream stream;
    stream << std::hex << id;
    return prefix + stream.str();
}


struct Message_ring_R: Ring_R<char>
{
    Message_ring_R(const string& directory, const string& prefix, uint64_t id):
        Ring_R(directory, get_base_filename(prefix, id), message_ring_size),
        m_id(id)
    {}


    void done_reading()
    {
        Ring_R::done_reading();
        m_range = decltype(m_range)();
    }

    // Returns a pointer to the buffer of the message
    // If there is no message to read, it blocks.
    Message_prefix* fetch_message()
    {
        // if all the messages in the reading buffer have been read
        if (m_range.begin == m_range.end) {
            // if this is not an uninitialized state
            if (m_reading) {
                // finalize the reading
                done_reading_new_data();
            }
            else {
                // initialize for all subsequent reads
                start_reading();
            }

            // start with a new reading buffer. this will block until there is something to read.
            auto range = wait_for_new_data();
            if (!range.begin) {
                return nullptr;
            }
            m_range = range;
        }

        bool f = false;
        while (!m_reading_lock.compare_exchange_strong(f, true)) { f = false; }

        //m_reading_lock
        if (!m_reading) {
            m_reading_lock = false;
            return nullptr;
        }

        Message_prefix* ret = (Message_prefix*)m_range.begin;
        assert(ret->magic == message_magic);
        m_range.begin += ret->bytes_to_next_message;

        m_reading_lock = false;
        return ret;
    }

    sequence_counter_type get_message_reading_sequence() const
    {
        return reading_sequence() - (m_range.end - m_range.begin);
    }


public:
    const uint64_t  m_id;

protected:
    Range<char>     m_range;
};



struct Message_ring_W: public Ring_W<char>
{
    Message_ring_W(const string& directory, const string& prefix, uint64_t id) :
        Ring_W(directory, get_base_filename(prefix, id), message_ring_size),
        m_id(id)
    {}

    Message_ring_W(const Message_ring_W&) = delete;
    const Message_ring_W& operator = (const Message_ring_W&) = delete;

    void relay(const Message_prefix& msg)
    {
        write((const char*)&msg, msg.bytes_to_next_message);
        done_writing();
    }

public:
    const uint64_t m_id;
};


  //\       //\       //\       //\       //\       //\       //\       //
 ////\     ////\     ////\     ////\     ////\     ////\     ////\     ////
//////\   //////\   //////\   //////\   //////\   //////\   //////\   //////
////////////////////////////////////////////////////////////////////////////
///// END MESSAGE RINGS ////////////////////////////////////////////////////
 //////////////////////////////////////////////////////////////////////////

} // namespace sintra

#endif
