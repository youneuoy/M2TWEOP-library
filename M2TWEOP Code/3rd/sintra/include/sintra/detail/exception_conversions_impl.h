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

#ifndef SINTRA_EXCEPTION_CONVERSION_IMPL_H
#define SINTRA_EXCEPTION_CONVERSION_IMPL_H


#include <system_error>
#include <regex>
#include <typeinfo>
#include <future>
#include <optional>

#include <boost/type_index/ctti_type_index.hpp>


namespace sintra {

template <
    typename T,
    typename /*= std::enable_if_t<is_base_of<std::exception, T>::value>*/
>
std::pair<type_id_type, std::string> exception_to_string(const T& ex)
{
    return std::make_pair(get_type_id<T>, ex.what());
}


template <
    typename T,
    typename /*= std::enable_if_t<!is_base_of<std::exception, T>::value>*/,
    typename /*= std::enable_if_t<std::is_same<std::invoke_result<decltype(std::to_string),T>::type, std::string>::value>*/
>
std::pair<type_id_type, std::string> exception_to_string(const T& ex)
{
    return std::make_pair(get_type_id<message_string>, std::to_string(ex));
}



template <
    typename T,
    typename /*= std::enable_if_t<!is_base_of<std::exception, T>::value>*/,
    typename /*= std::enable_if_t<!std::is_same<std::invoke_result<decltype(std::to_string),T>::type, std::string>::value>*/,
    typename /*= void*/
>
std::pair<type_id_type, std::string> exception_to_string(const T& ex)
{
    return std::make_pair(
        get_type_id<message_string>,
        std::string("Exception of type ") +
        boost::typeindex::ctti_type_index::template type_id<T>().pretty_name() +
        ", which is not serialized by sintra"
    );
}



//=======================



inline
void throw_generic_exception(const std::string& what)
{
    auto ex = (std::exception)std::runtime_error(what);
    throw ex;
}

template <typename T>
void throw_typed_exception(const std::string& what)
{
    throw T(what);
}
/*


// For the following exceptions, due to non-trivial constructor, a generic std::exception is thrown



{get_type_id<std::invalid_argument    >(), throw_typed_exception<std::invalid_argument >},
{get_type_id<std::domain_error        >(), throw_typed_exception<std::domain_error     >},
{get_type_id<std::length_error        >(), throw_typed_exception<std::length_error     >},
{get_type_id<std::out_of_range        >(), throw_typed_exception<std::out_of_range     >},
{get_type_id<std::range_error         >(), throw_typed_exception<std::range_error      >},
{get_type_id<std::overflow_error      >(), throw_typed_exception<std::overflow_error   >},
{get_type_id<std::underflow_error     >(), throw_typed_exception<std::underflow_error  >},
{get_type_id<std::ios_base::failure   >(), throw_typed_exception<std::ios_base::failure>},


{get_type_id<std::logic_error         >(), throw_typed_exception<std::logic_error      >},
{get_type_id<std::runtime_error       >(), throw_typed_exception<std::runtime_error    >},

// Due to their non-trivial constructor, the following exceptions are handled with their parent type
std::future_error         -> std::logic_error  
std::regex_error          -> std::runtime_error
std::system_error         -> std::runtime_error
fs::filesystem_error      -> std::runtime_error
std::bad_optional_access  -> std::exception
std::bad_typeid           -> std::exception
std::bad_cast             -> std::exception
std::bad_weak_ptr         -> std::exception
std::bad_function_call    -> std::exception
std::bad_alloc            -> std::exception
std::bad_array_new_length -> std::exception
std::bad_exception        -> std::exception
std::bad_variant_access   -> std::exception


*/

inline
void string_to_exception(type_id_type exception_type, const std::string& str)
{
    static unordered_map<type_id_type, void(*)(const std::string&)> ex_map = {
        {get_type_id<std::logic_error         >(), throw_typed_exception<std::logic_error      >},
        {get_type_id<std::invalid_argument    >(), throw_typed_exception<std::invalid_argument >},
        {get_type_id<std::domain_error        >(), throw_typed_exception<std::domain_error     >},
        {get_type_id<std::length_error        >(), throw_typed_exception<std::length_error     >},
        {get_type_id<std::out_of_range        >(), throw_typed_exception<std::out_of_range     >},
        {get_type_id<std::runtime_error       >(), throw_typed_exception<std::runtime_error    >},
        {get_type_id<std::range_error         >(), throw_typed_exception<std::range_error      >},
        {get_type_id<std::overflow_error      >(), throw_typed_exception<std::overflow_error   >},
        {get_type_id<std::underflow_error     >(), throw_typed_exception<std::underflow_error  >},
        {get_type_id<std::system_error        >(), throw_typed_exception<std::runtime_error    >},
        {get_type_id<std::ios_base::failure   >(), throw_typed_exception<std::ios_base::failure>},

        // For the following exceptions, due to non-trivial constructor, their parent type is thrown
        {get_type_id<std::future_error        >(), throw_typed_exception<std::logic_error      >},
        {get_type_id<std::regex_error         >(), throw_typed_exception<std::runtime_error    >},
        {get_type_id<fs::filesystem_error     >(), throw_typed_exception<std::runtime_error    >},

        // For the following exceptions, due to non-trivial constructor, a generic std::exception is thrown
        {get_type_id<std::bad_optional_access >(), throw_generic_exception},
        {get_type_id<std::bad_typeid          >(), throw_generic_exception},
        {get_type_id<std::bad_cast            >(), throw_generic_exception},
        {get_type_id<std::bad_weak_ptr        >(), throw_generic_exception},
        {get_type_id<std::bad_function_call   >(), throw_generic_exception},
        {get_type_id<std::bad_alloc           >(), throw_generic_exception},
        {get_type_id<std::bad_array_new_length>(), throw_generic_exception},
        {get_type_id<std::bad_exception       >(), throw_generic_exception},
        {get_type_id<std::bad_variant_access  >(), throw_generic_exception}
    };

    auto it = ex_map.find(exception_type);
    if (it != ex_map.end()) {
        it->second(str);
    }
    else {
        throw_generic_exception(str);
    }
}

} // namespae sintra

#endif
