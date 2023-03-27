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

#ifndef SINTRA_CALL_FUNCTION_WITH_TUPLE_ARGS_H
#define SINTRA_CALL_FUNCTION_WITH_TUPLE_ARGS_H


#include <tuple>
#include <type_traits>


namespace sintra {


using std::enable_if_t;
using std::get;
using std::tuple;
using std::tuple_size;


 //////////////////////////////////////////////////////////////////////////
///// BEGIN simple function backend ////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
//////   \//////   \//////   \//////   \//////   \//////   \//////   \//////
 ////     \////     \////     \////     \////     \////     \////     \////
  //       \//       \//       \//       \//       \//       \//       \//


template <
    typename FT,        // function type
    typename TT,        // tuple type
    typename... Args,
    typename = enable_if_t<
        tuple_size<TT>::value == sizeof...(Args)
    >
>
auto call_function_with_tuple_args_sfb(
    const FT& f,
    const TT& t,
    const Args&... args) -> decltype(f(args...))
{
    return f(args...);
}


template <
    typename FT,
    typename TT,
    typename... Args,
    typename = enable_if_t<
        tuple_size<TT>::value != sizeof...(Args)
    >
>
auto call_function_with_tuple_args_sfb(const FT& f, const TT& t, const Args&... args)
{
    return call_function_with_tuple_args_sfb(f, t, args..., get< sizeof...(Args) >(t));
}


template <
    typename FT,
    typename TT,
    typename = enable_if_t<
        tuple_size<TT>::value != 0
    >
>
auto call_function_with_tuple_args(const FT& f, const TT& t)
{
    // TODO: implement a static assertion for the statement that follows, to provide the message
    // in the compiler output.

    // if the compiler complains here, make sure that the function you are trying to call has
    // no non-const references
    return call_function_with_tuple_args_sfb(f, t, get<0>(t));
}


template <
    typename FT,
    typename TT,
    typename = enable_if_t<
        tuple_size<TT>::value == 0
    >
>
auto call_function_with_tuple_args(const FT& f, const TT& t) -> decltype(f())
{
    // the function is called without arguments
    return f();
}


  //\       //\       //\       //\       //\       //\       //\       //
 ////\     ////\     ////\     ////\     ////\     ////\     ////\     ////
//////\   //////\   //////\   //////\   //////\   //////\   //////\   //////
////////////////////////////////////////////////////////////////////////////
///// END simple function backend //////////////////////////////////////////
 //////////////////////////////////////////////////////////////////////////


 //////////////////////////////////////////////////////////////////////////
///// BEGIN member function backend ////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
//////   \//////   \//////   \//////   \//////   \//////   \//////   \//////
 ////     \////     \////     \////     \////     \////     \////     \////
  //       \//       \//       \//       \//       \//       \//       \//



template <
    typename TObj,
    typename FT,
    typename TT,
    typename... Args,
    typename = enable_if_t<
        tuple_size<TT>::value == sizeof...(Args)
    >
>
auto call_function_with_tuple_args_mfb(
    TObj& obj,
    const FT& f,
    const TT& t,
    const Args&... args) -> decltype((obj.*f)(args...))
{
    return (obj.*f)(args...);
}


template <
    typename TObj,
    typename FT,
    typename TT,
    typename... Args,
    typename = enable_if_t<
        tuple_size<TT>::value != sizeof...(Args)
    >
>
auto call_function_with_tuple_args_mfb(
    TObj& obj,
    const FT& f,
    const TT& t,
    const Args&... args)
{
    return call_function_with_tuple_args_mfb(obj, f, t, args..., get< sizeof...(Args) >(t));
}


template <
    typename TObj,
    typename FT,
    typename TT,
    typename = enable_if_t<
        tuple_size<TT>::value != 0
    >
>
auto call_function_with_tuple_args(TObj& obj, const FT& f, const TT& t)
{
    // TODO: implement a static assertion for the statement that follows, to provide the message
    // in the compiler output.

    // if the compiler complains here, make sure that the function you are trying to call has
    // no non-const references
    return call_function_with_tuple_args_mfb(obj, f, t, get<0>(t));
}


template <
    typename TObj,
    typename FT,
    typename TT,
    typename = enable_if_t<
        tuple_size<TT>::value == 0
    >
>
auto call_function_with_tuple_args(TObj& obj, FT f, const TT& t) -> decltype((obj.*f)())
{
    // the function is called without arguments
    return (obj.*f)();
}


  //\       //\       //\       //\       //\       //\       //\       //
 ////\     ////\     ////\     ////\     ////\     ////\     ////\     ////
//////\   //////\   //////\   //////\   //////\   //////\   //////\   //////
////////////////////////////////////////////////////////////////////////////
///// END member function backend //////////////////////////////////////////
 //////////////////////////////////////////////////////////////////////////


 //////////////////////////////////////////////////////////////////////////
///// BEGIN for_each_in_tuple //////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
//////   \//////   \//////   \//////   \//////   \//////   \//////   \//////
 ////     \////     \////     \////     \////     \////     \////     \////
  //       \//       \//       \//       \//       \//       \//       \//


// The following code was mostly taken from here (with some minor additions):
// http://stackoverflow.com/questions/16387354/template-tuple-calling-a-function-on-each-element


namespace detail_for_each_in_tuple
{
    template<int... Is>
    struct seq { };

    template<int N, int... Is>
    struct gen_seq : gen_seq<N - 1, N - 1, Is...> { };

    template<int... Is>
    struct gen_seq<0, Is...> : seq<Is...> { };

    template<typename T, typename F>
    void for_each(T&& t, F f, seq<>)
    {
    }

    template<typename T, typename F, int... Is>
    void for_each(T&& t, F f, seq<Is...>)
    {
        auto l = { (f(get<Is>(t)), 0)... };
    }

    template<typename T, typename F>
    void for_each_i(T&& t, F f, seq<>)
    {
    }

    template<typename T, typename F, int... Is>
    void for_each_i(T&& t, F f, seq<Is...>)
    {
        auto l = { (f(get<Is>(t), Is), 0)... };
    }
}

template<typename... Ts, typename F>
void for_each_in_tuple(tuple<Ts...> const& t, F f)
{
    detail_for_each_in_tuple::for_each(t, f, detail_for_each_in_tuple::gen_seq<sizeof...(Ts)>());
}

template<typename... Ts, typename F>
void for_each_in_tuple(tuple<Ts...> & t, F f)
{
    detail_for_each_in_tuple::for_each(t, f, detail_for_each_in_tuple::gen_seq<sizeof...(Ts)>());
}

template<typename... Ts, typename F>
void for_each_in_tuple_i(tuple<Ts...> const& t, F f)
{
    detail_for_each_in_tuple::for_each_i(t, f, detail_for_each_in_tuple::gen_seq<sizeof...(Ts)>());
}

template<typename... Ts, typename F>
void for_each_in_tuple_i(tuple<Ts...> & t, F f)
{
    detail_for_each_in_tuple::for_each_i(t, f, detail_for_each_in_tuple::gen_seq<sizeof...(Ts)>());
}



namespace tuple_zip_detail
{
    template<std::size_t ...S>
    struct seq { };

    template<std::size_t N, std::size_t ...S>
    struct gens : gens<N-1, N-1, S...> { };

    template<std::size_t ...S>
    struct gens<0, S...> {
        typedef seq<S...> type;
    };
}



template <
    template <typename ...> class Tup1,
    template <typename ...> class Tup2,
    typename ...A,
    typename ...B,
    std::size_t ...S
>
auto tuple_zip_helper(Tup1<A...> t1, Tup2<B...> t2, tuple_zip_detail::seq<S...> s) ->
decltype(std::make_tuple(std::make_pair(std::get<S>(t1), std::get<S>(t2))...))
{
    return std::make_tuple(std::make_pair(std::get<S>(t1), std::get<S>(t2))...);
}

template <
    template <typename ...> class Tup1,
    template <typename ...> class Tup2,
    typename ...A,
    typename ...B
>
auto tuple_zip(Tup1<A...> t1, Tup2<B...> t2) ->
decltype(tuple_zip_helper(t1, t2, typename tuple_zip_detail::gens<sizeof...(A)>::type() ))
{
      static_assert(sizeof...(A) == sizeof...(B), "The tuple sizes must be the same");
      return tuple_zip_helper( t1, t2, typename tuple_zip_detail::gens<sizeof...(A)>::type() );
}



} // namespace sintra


  //\       //\       //\       //\       //\       //\       //\       //
 ////\     ////\     ////\     ////\     ////\     ////\     ////\     ////
//////\   //////\   //////\   //////\   //////\   //////\   //////\   //////
////////////////////////////////////////////////////////////////////////////
///// END for_each_in_tuple ////////////////////////////////////////////////
 //////////////////////////////////////////////////////////////////////////




/*
// This is a test

#include <iostream>


using namespace sintra;


int func0()
{
    std::cout << "func0" << '\n';
    return 5;
}


void func1(int a)
{
    std::cout << a << '\n';
}


float func2(int a, float b)
{
    std::cout << a << ' ' << b << '\n';
    return 0.2f;
}


struct B
{
    int mfunc0()
    {
        std::cout << "mfunc0" << '\n';
        return 5;
    }

    void mfunc1(int a)
    {
        std::cout << a << '\n';
    }

    float mfunc2(int a, float b)
    {
        std::cout << a << ' ' << b << '\n';
        return 0.2f;
    }
};



int main(void)
{
    std::tuple<> t0 = std::make_tuple();
    std::tuple<int> t1 = std::make_tuple(1);
    std::tuple<int, float> t2 = std::make_tuple(1, 1.5f);

    auto rv0 = call_function_with_tuple_args(func0, t0);
    call_function_with_tuple_args(func1, t1);
    auto rv2 = call_function_with_tuple_args(func2, t2);

    std::cout << rv0 << ' ' << rv2 << '\n';

    B b;
    auto rv00 = call_function_with_tuple_args(b, &B::mfunc0, t0);
    call_function_with_tuple_args(b, &B::mfunc1, t1);
    auto rv22 = call_function_with_tuple_args(b, &B::mfunc2, t2);

    std::cout << rv00 << ' ' << rv22 << '\n';

    return 0;
}
*/


#endif
