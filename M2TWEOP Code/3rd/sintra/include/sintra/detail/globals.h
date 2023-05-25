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

#ifndef SINTRA_GLOBALS_H
#define SINTRA_GLOBALS_H


#include <cstdint>


namespace sintra {

using type_id_type = uint64_t;
using instance_id_type = uint64_t;


struct Managed_process;
struct Coordinator;

#if 0

// MSVC may under some circumstances generate wrong code with static inline data.
// When it's fixed, this block can replace the one below.
// Tested with MSVC 17.0.2 and 16.11.7

static inline Managed_process* s_mproc = nullptr;
static inline Coordinator* s_coord = nullptr;
static inline instance_id_type s_mproc_id = 0;
static inline instance_id_type s_coord_id = 0;

#else

// The purpose of this macro is to declare and define static data in a header-only library

#define DECLARE_STATIC_VARIABLE(type, name, ...)                \
    template <typename T = void>                                \
    struct name##_struct                                        \
    {                                                           \
        static type s;                                          \
    };                                                          \
    template <typename T> type name##_struct<T>::s __VA_ARGS__; \
    using name = name##_struct<void>;


DECLARE_STATIC_VARIABLE(Managed_process*, mproc, = nullptr)
DECLARE_STATIC_VARIABLE(Coordinator*, coord, = nullptr)
DECLARE_STATIC_VARIABLE(instance_id_type, mproc_id, = 0)
DECLARE_STATIC_VARIABLE(instance_id_type, coord_id, = 0)

#define s_mproc mproc::s
#define s_coord coord::s
#define s_mproc_id mproc_id::s
#define s_coord_id coord_id::s

#endif

}

#endif
