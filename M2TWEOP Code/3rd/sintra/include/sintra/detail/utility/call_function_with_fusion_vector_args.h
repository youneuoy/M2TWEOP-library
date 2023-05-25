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

#ifndef SINTRA_CALL_FUNCTION_WITH_FUSION_VECTOR_ARGS_H
#define SINTRA_CALL_FUNCTION_WITH_FUSION_VECTOR_ARGS_H


#include <type_traits>

#include <boost/fusion/sequence/intrinsic/at_c.hpp>
#include <boost/fusion/include/vector.hpp>


namespace sintra {

    
using std::enable_if_t;
using boost::fusion::at_c;
using boost::fusion::result_of::size;


 //////////////////////////////////////////////////////////////////////////
///// BEGIN simple function backend ////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
//////    //////    //////    //////    //////    //////    //////    //////
 ////      ////      ////      ////      ////      ////      ////      ////
  //        //        //        //        //        //        //        //


template <
    typename TFunction,
    typename TVector,
    typename... Args,
    typename = enable_if_t<
        size<TVector>::type::value == sizeof...(Args)
    >
>
auto call_function_with_fusion_vector_args_sfb(
    const TFunction& f, const TVector&,
    const Args&... args) -> decltype(f(args...))
{
    return f(args...);
}


template <
    typename TFunction,
    typename TVector,
    typename... Args,
    typename = enable_if_t<
        size<TVector>::type::value != sizeof...(Args)
    >
>
auto call_function_with_fusion_vector_args_sfb(
    const TFunction& f,
    const TVector& t,
    const Args&... args)
{
    return call_function_with_fusion_vector_args_sfb(
        f, t, args..., at_c< sizeof...(Args) >(t));
}


template <
    typename TFunction,
    typename TVector,
    typename = enable_if_t<
        size<TVector>::type::value != 0
    >
>
auto call_function_with_fusion_vector_args(const TFunction& f, const TVector& t)
{
    // TODO: implement a static assertion for the statement that follows, to provide the message
    // in the compiler output.

    // if the compiler complains here, make sure that the function you are trying to call has
    // no non-const references
    return call_function_with_fusion_vector_args_sfb(f, t, at_c<0>(t));
}


template <
    typename TFunction,
    typename TVector,
    typename = enable_if_t<
        size<TVector>::type::value == 0
    >
>
auto call_function_with_fusion_vector_args(const TFunction& f, const TVector&) -> decltype(f())
{
    // the function is called without arguments
    return f();
}



  //        //        //        //        //        //        //        //
 ////      ////      ////      ////      ////      ////      ////      ////
//////    //////    //////    //////    //////    //////    //////    //////
////////////////////////////////////////////////////////////////////////////
///// END simple function backend //////////////////////////////////////////
 //////////////////////////////////////////////////////////////////////////


 //////////////////////////////////////////////////////////////////////////
///// BEGIN member function backend ////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
//////    //////    //////    //////    //////    //////    //////    //////
 ////      ////      ////      ////      ////      ////      ////      ////
  //        //        //        //        //        //        //        //



template <
    typename TObj,
    typename TFunction,
    typename TVector,
    typename... Args,
    typename = enable_if_t<
        size<TVector>::type::value == sizeof...(Args)
    >
>
auto call_function_with_fusion_vector_args_mfb(
    TObj& obj,
    const TFunction& f,
    const TVector& /* t */,
    const Args&... args) -> decltype((obj.*f)(args...))
{
    return (obj.*f)(args...);
}


template <
    typename TObj,
    typename TFunction,
    typename TVector,
    typename... Args,
    typename = enable_if_t<
        size<TVector>::type::value != sizeof...(Args)
    >
>
auto call_function_with_fusion_vector_args_mfb(
    TObj& obj,
    const TFunction& f,
    const TVector& t,
    const Args&... args)
{
    return call_function_with_fusion_vector_args_mfb(
        obj, f, t, args..., at_c< sizeof...(Args) >(t));
}


template <
    typename TObj,
    typename TFunction,
    typename TVector,
    typename = enable_if_t<
        size<TVector>::type::value != 0
    >
>
auto call_function_with_fusion_vector_args(TObj& obj, const TFunction& f, const TVector& t)
{
    // TODO: implement a static assertion for the statement that follows, to provide the message
    // in the compiler output.

    // if the compiler complains here, make sure that none of the function arguments is either
    // - a non-const reference
    // - a non-POD and non STL container object
    return call_function_with_fusion_vector_args_mfb(obj, f, t, at_c<0>(t));
}


template <
    typename TObj,
    typename TFunction,
    typename TVector,
    typename = enable_if_t<
        size<TVector>::type::value == 0
    >
>
auto call_function_with_fusion_vector_args(
    TObj& obj,
    TFunction f,
    const TVector&) -> decltype((obj.*f)())
{
    // the function is called without arguments
    return (obj.*f)();
}



  //        //        //        //        //        //        //        //
 ////      ////      ////      ////      ////      ////      ////      ////
//////    //////    //////    //////    //////    //////    //////    //////
////////////////////////////////////////////////////////////////////////////
///// END member function backend //////////////////////////////////////////
 //////////////////////////////////////////////////////////////////////////


} // namespace sintra;



/*

// This is a test

#include <iostream>
#include <boost/fusion/include/make_vector.hpp>


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
    boost::fusion::vector<> t0 = boost::fusion::make_vector();
    boost::fusion::vector<int> t1 = boost::fusion::make_vector(1);
    boost::fusion::vector<int, float> t2 = boost::fusion::make_vector(1, 1.5f);

    auto rv0 = call_function_with_fusion_vector_args(func0, t0);
    call_function_with_fusion_vector_args(func1, t1);
    auto rv2 = call_function_with_fusion_vector_args(func2, t2);

    std::cout << rv0 << ' ' << rv2 << '\n';

    B b;
    auto rv00 = call_function_with_fusion_vector_args(b, &B::mfunc0, t0);
    call_function_with_fusion_vector_args(b, &B::mfunc1, t1);
    auto rv22 = call_function_with_fusion_vector_args(b, &B::mfunc2, t2);

    std::cout << rv00 << ' ' << rv22 << '\n';

    return 0;
}

*/


#endif
