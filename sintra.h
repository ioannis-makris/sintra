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

#ifndef __SINTRA_H__
#define __SINTRA_H__


#include "pm/managed_process.h"
#include "pm/coordinator.h"
#include "pm/transceiver.h"

#include "pm/globals.h"

#include "pm/managed_process_impl.h"
#include "pm/coordinator_impl.h"
#include "pm/transceiver_impl.h"
#include "pm/process_message_reader_impl.h"
#include "pm/resolvable_instance_impl.h"
#include "pm/message_impl.h"


namespace sintra
{


template <typename ...Args>
void start(int argc, char* argv[], const Process_descriptor& first, Args&&... rest);


// Stops all threads to allow the program to terminate.
void stop();


// Blocks the calling thread, until another thread calls stop()
void wait_for_stop();


// Blocks the calling thread of the calling process, until at least one thread of each processes
// in the the specified process group has called barrier().
// If multiple threads in each process call barrier(), they will take turns matching corresponding
// threads from other processes in the same group, in undefined matching order.
// Note that this is an interprocess synchronization mechanism. When it is sought to synchronize
// threads as well as processes, an additional thread synchronization mechanism must be used.
template <typename PROCESS_GROUP_TYPE = Externally_coordinated>
void barrier();


template <typename FT>
auto activate_slot(const FT& slot_function, instance_id_type sender_id = any_local_or_remote);


bool deactivate_slot(type_id_type message_id);


} // namespace sintra


#include "pm/sintra_impl.h"


#endif
