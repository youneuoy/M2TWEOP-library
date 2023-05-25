/*
Copyright 2021 Ioannis Makris

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

#ifndef SINTRA_EXCEPTION_CONVERSION_H
#define SINTRA_EXCEPTION_CONVERSION_H


#include <algorithm>
#include <exception>
#include <string>
#include <type_traits>

#include "globals.h"


namespace sintra {

template<typename = void, typename... Args>
struct is_string_convertible : std::false_type {};

template<typename... Args>
struct is_string_convertible<std::void_t<decltype(std::to_string(std::declval<Args>()...))>, Args...>
    : std::true_type {};

template<typename... Args>
inline constexpr bool is_string_convertible_v = is_string_convertible<void, Args...>::value;

template <
    typename T,
    typename = std::enable_if_t<std::is_base_of<std::exception, T>::value>
>
std::pair<type_id_type, std::string> exception_to_string(const T& ex);

template <
    typename T,
    typename = std::enable_if_t<!std::is_base_of<std::exception, T>::value>,
    typename = std::enable_if_t<is_string_convertible_v<T> >
>
std::pair<type_id_type, std::string> exception_to_string(const T& ex);

template <
    typename T,
    typename = std::enable_if_t<!std::is_base_of<std::exception, T>::value>,
    typename = std::enable_if_t<!is_string_convertible_v<T> >,
    typename = void
>
std::pair<type_id_type, std::string> exception_to_string(const T& ex);

inline
void string_to_exception(type_id_type exception_type, const char* str, size_t num_chars);

}


#endif