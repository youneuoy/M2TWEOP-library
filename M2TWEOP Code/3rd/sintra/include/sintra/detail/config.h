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

#ifndef SINTRA_CONFIG_H
#define SINTRA_CONFIG_H

#include <cstddef>

// Ring reading policies
// =====================

// The reading thread will yield if there is nothing to read. This means that the loop will
// be driven by OS-level interprocess synchronization, which may have some impact
// in performance. However, in single-processor systems, this policy will perform much
// better than the others.

#define SINTRA_RING_READING_POLICY_ALWAYS_SLEEP     0


// Using this policy, the reader will livelock while waiting for data. As a consequence, this
// policy does not require locking on either reading or writing.
// NOTE: It needs to be used with care and only when necessary, to avoid wasting CPU resources.

#define SINTRA_RING_READING_POLICY_ALWAYS_SPIN      1


// With this policy, the reading loop will initially spin once a read is made, but will
// eventually sleep after a certain time of inactivity.
// NOTE: This policy assumes that omp_get_wtime() is faster than locking an interprocess_mutex.
// The aforementioned condition would very likely be true for an x86 implementation using rdtsc.
// But should this not be the case on a given system, then this policy might eventually provide
// inferior performance.

#define SINTRA_RING_READING_POLICY_HYBRID           2


#define SINTRA_RING_READING_POLICY SINTRA_RING_READING_POLICY_HYBRID

#ifndef __clang__ 
#define SINTRA_USE_OMP_GET_WTIME
#endif

namespace sintra {

    // instance_id_type is a 2-part integer variable. The first part identifies the process and
    // the second the transceiver in the process. This variable configures the number of bits
    // allocated to the process index part and consequently the number of bits of transceiver
    // part as well.
    // NOTE: This value affects the size of Ring::Control exponentially.
    // see definition of instance_id_type
    constexpr int       num_process_index_bits              = 8;

    // The reason for setting such restriction is that the coordinator has to keep track of all
    // globally visible (public) transceivers. If a process is misbehaving and allocates public
    // transceivers carelessly, this could compromise the stability of the system
    constexpr int       max_public_transceivers_per_proc    = 65535;

    // Similarly, there is no true restriction to what this value could be. However, allowing a
    // process to send messages arbitrarily large could compromise stability.
    constexpr int       max_message_length                  = 4096;

    // This is a time value in seconds that the hybrid reading policy algorithm will try to approach
    // while spinning.
    // If any policy other than SINTRA_RING_READING_POLICY_HYBRID is used, the value is irrelevant.
    constexpr double    spin_before_sleep                   = 0.01;   // secs

    // Whenever control data is read and written in an array by multiple threads, the layout used
	// should not cause cache invalidations (false sharing). This setting is architecture specific,
	// but it's not really that different among different x86 CPUs.
    constexpr size_t    assumed_cache_line_size             = 0x40;
}


#endif
