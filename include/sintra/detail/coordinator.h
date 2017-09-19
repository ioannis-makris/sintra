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

#ifndef __SINTRA_COORDINATOR_H__
#define __SINTRA_COORDINATOR_H__


#include "resolvable_instance.h"
#include "resolve_type.h"
#include "transceiver.h"

#include <mutex>
#include <condition_variable>


namespace sintra {


using std::condition_variable;
using std::mutex;
using std::string;
using std::unordered_set;


struct Coordinator: public Transceiver
{
    TRANSCEIVER_PROLOGUE(Coordinator)

private:
    Coordinator();
    ~Coordinator();

    bool add_process_into_group(instance_id_type process_id, type_id_type process_group_id);
    void wait_until_all_other_processes_are_done();

    // EXPORTED FOR RPC
    type_id_type resolve_type(const string& name);
    instance_id_type resolve_instance(const string& name);
    bool publish_transceiver(instance_id_type instance_id, const string& name);
    bool unpublish_transceiver(instance_id_type instance_id);
    bool barrier(type_id_type process_group_id);
    bool add_this_process_into_group(type_id_type process_group_id);
    void print(const string& str);

    struct Barrier
    {
        mutex m;
        condition_variable cv;
        uint32_t processes_reached = 0;
    };

    spinlocked_map<type_id_type, Barrier >      m_barriers;
    mutex                                       m_barrier_mutex;

    spinlocked_map<
        instance_id_type,
        unordered_set< instance_id_type >
    >                                           m_published;
    mutex                                       m_publish_mutex;

    mutex                                       m_all_other_processes_done_mutex;
    condition_variable                          m_all_other_processes_done_condition;

    spinlocked_map<instance_id_type, string>    m_name_of_instance_id;

    spinlocked_map<
        instance_id_type, 
        spinlocked_set< instance_id_type >
    >                                           m_processes_of_group;
    spinlocked_map<
        instance_id_type,
        spinlocked_set< instance_id_type >
    >                                           m_groups_of_process;

public:
    EXPORT_RPC_EXPLICIT(resolve_type)  
    EXPORT_RPC_EXPLICIT(resolve_instance)
    EXPORT_RPC_EXPLICIT(publish_transceiver)
    EXPORT_RPC_EXPLICIT(unpublish_transceiver)
    EXPORT_RPC_EXPLICIT(barrier)
    EXPORT_RPC_EXPLICIT(add_this_process_into_group)
    EXPORT_RPC_EXPLICIT(print)

    friend struct Managed_process;
    friend struct Transceiver;
};

}


#endif
