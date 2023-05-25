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

#ifndef SINTRA_RESOLVABLE_INSTANCE_H
#define SINTRA_RESOLVABLE_INSTANCE_H


#include "message.h"


namespace sintra {


using std::string;


struct Resolvable_instance_id: Sintra_message_element
{
    Resolvable_instance_id(instance_id_type v): value(v) {}
    const Resolvable_instance_id& operator= (instance_id_type v)
                                                    { value = v; return *this;          }
    Resolvable_instance_id(const string& str)       { from_string(str);                 }
    Resolvable_instance_id(const char* str)         { from_string(str);                 }
    const Resolvable_instance_id& operator= (const string& str)
                                                    { from_string(str); return *this;   }
    operator instance_id_type() const               { return value;                     }

private:
    instance_id_type value = invalid_instance_id;

    template<typename = void>
    void from_string(const string& str);
};


} // sintra


#endif
