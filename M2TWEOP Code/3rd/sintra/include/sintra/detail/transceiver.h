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

#ifndef SINTRA_TRANSCEIVER
#define SINTRA_TRANSCEIVER


#include "globals.h"
#include "id_types.h"
#include "message.h"
#include "spinlocked_containers.h"

#include <condition_variable>
#include <functional>
#include <list>
#include <map>
#include <mutex>
#include <unordered_map>

#include <boost/bind/bind.hpp>
#include <boost/type_traits.hpp>


namespace sintra {


using std::condition_variable;
using std::function;
using std::is_base_of;
using std::is_const;
using std::is_reference;
using std::list;
using std::map;
using std::mutex;
using std::recursive_mutex;
using std::remove_reference;
using std::string;
using std::unordered_map;




 //////////////////////////////////////////////////////////////////////////
///// BEGIN Typed_instance_id //////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
//////   \//////   \//////   \//////   \//////   \//////   \//////   \//////
 ////     \////     \////     \////     \////     \////     \////     \////
  //       \//       \//       \//       \//       \//       \//       \//


template <typename T>
struct Typed_instance_id
{
    instance_id_type    id;
    Typed_instance_id(const T& transceiver)
    {
        id = transceiver.m_instance_id;
    }


    Typed_instance_id(instance_id_type id_)
    {
        id = id_;
    }
};


template <typename T>
Typed_instance_id<T> make_typed_instance_id(const T& transceiver)
{
    return Typed_instance_id<T>(transceiver);
}


// This specialization applies to non-typed generic groups such as any_local etc.
template<>
struct Typed_instance_id<void>
{
    instance_id_type    id;
    Typed_instance_id(instance_id_type id_)
    {
        id = id_;
    }
};


inline
Typed_instance_id<void> make_untyped_instance_id(instance_id_type naked_instance_id)
{
    return Typed_instance_id<void>(naked_instance_id);
}


  //\       //\       //\       //\       //\       //\       //\       //
 ////\     ////\     ////\     ////\     ////\     ////\     ////\     ////
//////\   //////\   //////\   //////\   //////\   //////\   //////\   //////
////////////////////////////////////////////////////////////////////////////
///// END Typed_instance_id ////////////////////////////////////////////////
 //////////////////////////////////////////////////////////////////////////



struct tn_type
{
    type_id_type                            type_id = 0;
    string                                  name;

    bool operator==(const tn_type& rhs) const
    {
        return
            type_id == rhs.type_id &&
            name    == rhs.name;
    }
};


} // leaving sintra namespace temporarily, to define the hasher

namespace std
{
    template <>
    struct hash<sintra::tn_type>
    {
        uint64_t operator()(const sintra::tn_type& k) const
        {
            uint64_t res = 17;
            res = res * 31 + hash<sintra::type_id_type>()(k.type_id);
            res = res * 31 + hash<string>()(k.name);
            return res;
        }
    };
}


namespace sintra { // continue with sintra namespace


template <typename T>
struct Named_instance: std::string
{
    Named_instance(const std::string& rhs) : std::string(rhs) {}
    Named_instance(std::string&& rhs) : std::string(std::move(rhs)) {}
};



using handler_proc_registry_mid_record_type = 
    spinlocked_umap <
        instance_id_type,                                // sender
        list<function<void(const Message_prefix &)>>
    >;


using handler_registry_type =
    spinlocked_umap <
        type_id_type,                                    // message type
        handler_proc_registry_mid_record_type
    >;


struct Transceiver
{
    using Transceiver_type = Transceiver;

    template <typename = void>
    Transceiver(const string& name = "", uint64_t id = 0);

    ~Transceiver();

    inline
    instance_id_type instance_id() { return m_instance_id; }

    template <typename = void>
    bool assign_name(const string& name);

    template <typename = void>
    void destroy();

protected:

    // Used when there is no infrastructure to initialize Transceiver, thus the caller
    // (Managed_process) calls the other constructor explicitly, when construction is possible.
    Transceiver(void*) {}

    template <typename = void>
    void construct(const string& name = "", uint64_t id = 0);

public:

    SINTRA_SIGNAL_EXPLICIT(
        exception,
        message_string what
    )

    SINTRA_SIGNAL_EXPLICIT(
        deferral,
        instance_id_type new_fiid
    )

    list<function<void()>> m_deactivators;

    using handler_deactivator = std::function<void()>;

    template<typename MESSAGE_T, typename HT>
    handler_deactivator activate_impl(
        HT&& handler,
        instance_id_type sender_id,
        decltype(m_deactivators)::iterator* deactivator_it_ptr = nullptr);


    // A functor with an arbitrary non-message argument
    template<
        typename SENDER_T,
        typename FT,
        typename = decltype(&FT::operator()),  //must be functor
        typename FUNCTOR_ARG_T = decltype(resolve_single_functor_arg(*((FT*)0))),

        // prevent functors with message arguments from matching the template
        typename = enable_if_t<
            !is_base_of<
                Message_prefix,
                typename remove_reference<FUNCTOR_ARG_T>::type
            >::value
        >
    >
    handler_deactivator activate(
        const FT& internal_slot,
        Typed_instance_id<SENDER_T> sender_id,
        decltype(m_deactivators)::iterator* deactivator_it_ptr = nullptr);


    // A functor with a message argument
    template<
        typename SENDER_T,
        typename FT,
        typename = decltype(&FT::operator()),    //must be functor
        typename = void, // differentiate from the previous template
        typename FUNCTOR_ARG_T = decltype(resolve_single_functor_arg(*((FT*)0))),

        // only allow functors with message arguments to match the template
        typename = enable_if_t<
            is_base_of<
                Message_prefix,
                typename remove_reference<FUNCTOR_ARG_T>::type
            >::value
        >
    >
    handler_deactivator activate(
        const FT& internal_slot,
        Typed_instance_id<SENDER_T> sender_id,
        decltype(m_deactivators)::iterator* deactivator_it_ptr = nullptr);


    // A Transceiver member function with a message argument. The sender has to exist.
    template<
        typename SENDER_T,
        typename MESSAGE_T,
        typename OBJECT_T,
        typename RT = typename MESSAGE_T::return_type
    >
    handler_deactivator activate(
        RT(OBJECT_T::*v)(const MESSAGE_T&), 
        Typed_instance_id<SENDER_T> sender_id,
        decltype(m_deactivators)::iterator* deactivator_it_ptr = nullptr);


    // Any kind of slot (member or function) will be accepted here.
    // By default, the sender does not have to exist. If it does not exist, the
    // Coordinator is notified, and once the conditions are met, it will send a signal
    // back, to trigger a new activation attempt. This behaviour may be disabled,
    // by specifying 'true' in the first template parameter.
    template<
        bool sender_must_exist = false,
        typename SLOT_T,
        typename SENDER_T
    >
    handler_deactivator // unlike its overloads, this one does not return pointer
    activate(
        const SLOT_T& rcv_slot,
        Named_instance<SENDER_T> sender);


    template <typename = void>
    void deactivate_all();

    
    template <
        typename MESSAGE_T,
        instance_id_type LOCALITY,
        typename SENDER_T,
        typename... Args>
    void send(Args&&... args);



 //////////////////////////////////////////////////////////////////////////
///// BEGIN RPC ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
//////   \//////   \//////   \//////   \//////   \//////   \//////   \//////
 ////     \////     \////     \////     \////     \////     \////     \////
  //       \//       \//       \//       \//       \//       \//       \//


    // The following code was originally developed in MSVC2015 Update 2.
    // Please be aware that some design decisions aim to prevent compiler errors.
    // (for example, the resolution_dummy could have been avoided - the export function does
    // not have to be variadic, nor is it really necessary to have two copies of export to match
    // the const)

    template <
        typename MF,
        MF m,
        type_id_type ID,
        bool MAY_BE_CALLED_DIRECTLY,
        typename RT = decltype(resolve_rt(m)),
        typename OBJECT_T = decltype(resolve_object_type(m))
    >
    struct RPCTC_d // remote process call type container
    {
        using mf_type = MF;
        using r_type = RT;
        using o_type = OBJECT_T;
        const static MF mf() { return m; };
        static constexpr type_id_type id = ID;
        static constexpr bool may_be_called_directly = MAY_BE_CALLED_DIRECTLY;
    };


    // Short note: It is fairly simple to change the message body from boost::fusion::vector to
    // std::tuple, to avoid yet another boost dependency, but be aware of the implications.
    // The implementation of fusion::vector is a plain struct. The tuple on the other hand will most
    // likely use recursive inheritance, which may produce different memory layout. In some cases,
    // the fusion::vector will be more compact. For example, the following
    // 
    //     std::cout <<
    //        sizeof(boost::fusion::vector<int, char, double, char, short, int>) << ' ' <<
    //        sizeof(std::tuple           <int, char, double, char, short, int>);
    //
    // on my system would print
    // 24 40


    template <
        typename RPCTC,   // <- the m in RPCTC makes it unique
        typename... Args
    >
    struct unique_message_body: serializable_type<boost::fusion::vector<Args...> >
    {
        using bfvec = serializable_type<boost::fusion::vector<Args...> >;
        using bfvec::bfvec;
    };



    template <typename RT>
    void warn_about_reference_return()
    {
        static_assert(!is_reference<RT>::value,
            "A function returning a reference cannot be exported for RPC. "
            "Read Sintra documentation for details.");
    }


    template <typename SEQ_T, int I, int J>
    struct warn_about_reference_args_impl {

        using arg_type = typename boost::fusion::result_of::value_at_c<SEQ_T, I>::type;

        static_assert(
            !is_reference<arg_type>::value ||
            is_const<typename remove_reference<arg_type>::type>::value,
            "A function with non-const reference arguments cannot be exported for RPC. "
            "Read Sintra documentation for details."
            );
        warn_about_reference_args_impl<SEQ_T, I + 1, J> dummy;
    };

    template <typename SEQ_T, int I>
    struct warn_about_reference_args_impl<SEQ_T, I, I> {};

    template <typename SEQ_T>
    void warn_about_reference_args()
    {
        warn_about_reference_args_impl<SEQ_T, 0, boost::fusion::result_of::size<SEQ_T>::value>();
    }


    // This is only for RPC, thus static. Since the handler maps to a class/struct function, all
    // instances of this class/struct use the same handler, which is just a function enclosing
    // a __thiscall to whichever member function was exported.
    inline
    static auto& get_rpc_handler_map();


    template <typename RPCTC>
    static auto& get_instance_to_object_map();


    template <typename R_MESSAGE_T, typename MESSAGE_T, typename OBJECT_T>
    static void finalize_rpc_write(
        R_MESSAGE_T* placed_rep_msg,
        const MESSAGE_T& req_msg,
        const OBJECT_T* ref_obj,
        type_id_type ex_tid);


    // This overload is meant to be directly used in special cases,
    // e.g. when returning to multiple recipients.
    // In this version the receiver and function instance ids are explicitly specified,
    // rather than deduced from the message, thus they override what might already be
    // specified in the message.
    template <typename R_MESSAGE_T, typename OBJECT_T>
    static void finalize_rpc_write(
        R_MESSAGE_T* placed_rep_msg,
        instance_id_type receiver_iid,
        instance_id_type function_iid,
        const OBJECT_T* ref_obj,
        type_id_type ex_tid);

    //template <
    //    typename RPCTC,
    //    typename MESSAGE_T,
    //    typename = void,
    //    typename = enable_if_t<is_same< typename RPCTC::r_type, void>::value>
    //>
    //static void rpc_handler(Message_prefix& untyped_msg);


    template <
        typename RPCTC,
        typename MESSAGE_T
    >
    static void rpc_handler(Message_prefix& untyped_msg);


    template <
        typename RPCTC,
        typename RT,
        typename OBJECT_T,
        typename... FArgs,      // The argument types of the exported member function
        typename... RArgs        // The argument types used by the caller
    >
    static RT rpc(
        RT(OBJECT_T::* /*resolution dummy arg*/)(FArgs...),
        instance_id_type instance_id,
        RArgs&&... args);


    template <
        typename RPCTC,
        typename RT,
        typename OBJECT_T,
        typename... FArgs,
        typename... RArgs
    >
    static RT rpc(
        RT(OBJECT_T::* /*resolution dummy arg*/)(FArgs...) const,
        instance_id_type instance_id,
        RArgs&&... args);


    template <
        typename RPCTC,
        typename MESSAGE_T,
        typename... Args
    >
    static auto rpc_impl(instance_id_type instance_id, Args... args) -> typename RPCTC::r_type;



    struct Return_handler
    {
        function<void(const Message_prefix&)>   return_handler;
        function<void(const Message_prefix&)>   exception_handler;
        function<void(const Message_prefix&)>   deferral_handler;
        instance_id_type                        instance_id = 0;
    };


    template<typename=void>
    instance_id_type activate_return_handler(const Return_handler &rh);

    inline
    void deactivate_return_handler(instance_id_type function_instance_id);

    // useful for deferred
    void replace_return_handler_id(instance_id_type old_id, instance_id_type new_id);


    template <typename RPCTC, typename MT>
    function<void()> export_rpc_impl();

    template <typename RPCTC, typename RT, typename OBJECT_T, typename... Args>
    function<void()> export_rpc(RT(OBJECT_T::* /*resolution dummy arg*/)(Args...) const);

    template <typename RPCTC, typename RT, typename OBJECT_T, typename... Args>
    function<void()> export_rpc(RT(OBJECT_T::* /*resolution dummy arg*/)(Args...));



#define SINTRA_RPC_IMPL(m, mfp, id, mbcd)                                                       \
    void rpc_assertion_##m() {                                                                  \
        static_assert(std::is_same_v<                                                           \
            std::remove_pointer_t<decltype(this)>,                                              \
            Transceiver_type>,                                                                  \
            "This Transceiver is not derived correctly."                                        \
        );                                                                                      \
    }                                                                                           \
    using m ## _mftc = RPCTC_d<decltype(mfp), mfp, id, mbcd>;                                   \
    sintra::Instantiator m ## _itt = export_rpc<m ## _mftc>(mfp);                               \
                                                                                                \
    template<typename... Args>                                                                  \
    static auto rpc_ ## m (sintra::Resolvable_instance_id instance_id, Args&&... args)          \
    {                                                                                           \
        return rpc<m ## _mftc>(mfp, instance_id, args...);                                      \
    }

    
    // Exports a member function for RPC.
    #define SINTRA_RPC(m)                                                                       \
        SINTRA_RPC_IMPL(m, &Transceiver_type :: m, sintra::invalid_type_id, true)

    // Exports a member function for RPC, provided that an ID is already reserved for a function
    // with that name. This is only meant to be used internally.
    #define SINTRA_RPC_EXPLICIT(m)                                                              \
        SINTRA_RPC_IMPL(m, &Transceiver_type :: m, (type_id_type)sintra::detail::reserved_id::m, true)

    // Exports a member function exclusively for RPC use. This implies that the function is
    // written in a way that makes no sense to call it directly. For example, this could be the
    // case if it performs some collective operation (i.e. an operation that involves multiple
    // processes, such as process barriers and scatter/gather), or returns asynchronously.
    // Functions which are meant to be used exclusively via RPC, will not take a direct-call
    // shortcut when called within the same process, but will instead use the RPC mechanism
    // in all cases.
    #define SINTRA_RPC_STRICT(m)                                                                  \
        SINTRA_RPC_IMPL(m, &Transceiver_type :: m, sintra::invalid_type_id, false)

    // Exports a member function exclusively for RPC, provided that an ID is already reserved for
    // a function with that name. This is only meant to be used internally.
    #define SINTRA_RPC_STRICT_EXPLICIT(m)                                                         \
        SINTRA_RPC_IMPL(m, &Transceiver_type :: m, (type_id_type)sintra::detail::reserved_id::m, false)

  //\       //\       //\       //\       //\       //\       //\       //
 ////\     ////\     ////\     ////\     ////\     ////\     ////\     ////
//////\   //////\   //////\   //////\   //////\   //////\   //////\   //////
////////////////////////////////////////////////////////////////////////////
///// END RPC //////////////////////////////////////////////////////////////
 //////////////////////////////////////////////////////////////////////////


private:

    // Handlers of return messages (i.e. messages delivering the results of function messages).
    // Note that the key is an instance_id_type, rather than a type_id_type.
    // Those message handlers identify with particular function message invocations, and their
    // lifetime ends with the end of the call.
    // They are assigned in pairs, to handle successful and failed calls.
    mutex m_return_handlers_mutex;
    unordered_map<instance_id_type, Return_handler> m_active_return_handlers;


    instance_id_type            m_instance_id       = invalid_instance_id;
    bool                        m_published         = false;

    spinlocked_umap<string, instance_id_type>::iterator m_cache_iterator;


protected:

    // WARNING: this will be set when the Transceiver is published.
    // If it is not published, it will remain invalid.
    type_id_type                m_type_id           = invalid_type_id;
    function<void()>            initialize_type_id  = [this]()
    {
        m_type_id = get_type_id<Transceiver_type>();
    };


    friend struct Managed_process;
    friend struct Coordinator;
    friend struct Process_message_reader;

    template<typename>
    friend struct Typed_instance_id;

    template<typename , typename>
    friend struct Derived_transceiver;
};



struct Empty_struct{};


// Second or higher level inheritance (i.e. inherits from a parent
// who is already derived from Transceiver)
template<typename Derived_T, typename Parent=void>
struct Derived_transceiver: Parent
{
    //using Transceiver::Transceiver;

    Derived_transceiver():
        base( *static_cast<Transceiver*>(static_cast<Derived_T*>(this)) )
    {
    }

    using Transceiver_type = Derived_T;

    template <
        typename MESSAGE_T,
        typename SENDER_T = Transceiver_type,
        typename... Args>
        void emit_local(Args&&... args)
    {
        base.send<MESSAGE_T, any_local, SENDER_T>(std::forward<Args>(args)...);
    }

    template <
        typename MESSAGE_T,
        typename SENDER_T = Transceiver_type,
        typename... Args>
        void emit_remote(Args&&... args)
    {
        base.send<MESSAGE_T, any_remote, SENDER_T>(std::forward<Args>(args)...);
    }

    template <
        typename MESSAGE_T,
        typename SENDER_T = Transceiver_type,
        typename... Args>
        void emit_global(Args&&... args)
    {
        base.send<MESSAGE_T, any_local_or_remote, SENDER_T>(std::forward<Args>(args)...);
    }


    static Named_instance<Transceiver_type> named_instance(const std::string& name)
    {
        return name;
    }

private:
    Transceiver& base;

    int type_initializer()
    {
        base.initialize_type_id = [this]() {
            base.m_type_id = get_type_id<Transceiver_type>();
        };
        return 0;
    }
    int itidi = type_initializer(); // initialize the initialize_type_id
};



// First level inheritance, after Transceiver (thus has to inherit from Transceiver)
template<typename Derived_T>
struct Derived_transceiver<Derived_T, void>: Transceiver, Derived_transceiver<Derived_T, Empty_struct>
{
    using Transceiver::Transceiver;
    using Transceiver_type = Derived_T;

private:

    int type_initializer()
    {
        initialize_type_id = [this]() {
            m_type_id = get_type_id<Transceiver_type>();
        };
        return 0;
    }
    int itidi = type_initializer(); // initialize the initialize_type_id
};



struct Outstanding_rpc_control
{
    mutex               keep_waiting_mutex;
    condition_variable  keep_waiting_condition;

    // this is set before the rpc is written to the ring, in rpc_impl.
    // It is meant to identify the remote instance, in case it
    // crashes, in order to unblock the local caller.
    instance_id_type    remote_instance = invalid_instance_id;

    bool                keep_waiting = true;
    bool                success = false;
};



} // sintra


#endif
