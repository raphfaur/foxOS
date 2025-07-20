#include "./scheduler.h"
#include "../../drivers/timer/timer.h"
#include "../../drivers/gic/gic.h"
#include "../utils/debug.h"
#include "./tasks.h"
#include "string.h"

struct Scheduler scheduler = {
    .registered_tasks = {0}
};

int __init(struct Scheduler *scheduler) {
    scheduler->current_pstate = 0;
    scheduler->task_n = 0;
    scheduler->current_return_address = 0;
    scheduler->current_task_index = 0;
    scheduler->current_stack_address = 0;
    scheduler->initiated = 0;
    for (int i = 0; i < 16; i++)
    {
        scheduler->regs[i] = 0;
    }
    
    return 0;
};

void __start(struct Scheduler* scheduler) {
    set_timer(100000000);
    enable_timer_int();
    while (1){}
};

scheduler_error register_task(struct Task* t) {
    DEBUG("Registering task")
    if (scheduler.task_n == MAX_TASK) {
        return SCHEDULER_ERROR;
    }
    scheduler.registered_tasks[scheduler.task_n] = t;
    scheduler.task_n ++;
    return SCHEDULER_SUCCESS;
}

void _set_userland_stack_address(void * stack_address) {
    uint64_t actual_address;
    DEBUG("Setting new stack adress")
    __asm("MSR SP_EL0, %[stack_address]" : : [stack_address]"r"((uint64_t) stack_address));
    __asm("MRS %[stack_address], SP_EL0" : [stack_address]"=r"(actual_address):);
    DEBUGH(actual_address)
}

void set_userland_stack_pointer() {
    scheduler.current_pstate &= ~0b1111UL;
}

uint64_t get_userland_stack_pointer() {
    uint64_t userland_pointer;
    __asm("MRS %[stack_address], SP_EL0" : [stack_address]"=r"(userland_pointer):);
    return userland_pointer;
}

scheduler_error _switch_task_context(int prev_task_id, int next_task_id , uint64_t pstate, uint64_t elr) {
    DEBUG("Switching task context")
    struct Task *prev_task = scheduler.registered_tasks[prev_task_id];
    struct Task *next_task = scheduler.registered_tasks[next_task_id];

    
    if (scheduler.initiated == 0) {
        // First task to launch
        DEBUG("Initialization")
        prev_task->pstate = pstate;
        scheduler.current_pstate = pstate;
        scheduler.current_return_address = (uint64_t) scheduler.registered_tasks[0]->run;
        scheduler.current_stack_address = scheduler.registered_tasks[0]->stak_address;
        set_userland_stack_pointer();
        return SCHEDULER_SUCCESS;

    } else {
        if (prev_task == NULL || next_task == NULL) 
        {
            return SCHEDULER_ERROR;
        }
    
        DEBUG("Saving PSTATE")
        // Set PSTATE of old task
        prev_task->pstate = pstate;
        DEBUGH(prev_task->pstate)
        DEBUG("Saving ELR")

        // Set return address of old task
        prev_task->return_address = elr;
        DEBUGH(prev_task->return_address);

        // Set SP of old task
        DEBUG("Saving stack adresss")
        prev_task->stak_address = (void *) get_userland_stack_pointer();
        DEBUGH((uint64_t)prev_task->stak_address)

        // Save registers
        memcpy(prev_task->regs, scheduler.regs, sizeof(scheduler.regs));

        // Handle the case where the task has never been called
        if (next_task->valid) {
            DEBUG("Next task valid, restoring context")
            // Get next task PSTATE
            scheduler.current_pstate = next_task->pstate;

            // Get next return address
            scheduler.current_return_address = next_task->return_address;
            set_userland_stack_pointer();

            // Set next task registers
            memcpy(scheduler.regs, next_task->regs, sizeof(scheduler.regs));
   
            DEBUGH(scheduler.current_pstate)
            DEBUGH(scheduler.current_return_address)
            // Set next stack pointer

        } else {

            DEBUG("Next task not valid, setting new address")
            scheduler.current_pstate = pstate;
            scheduler.current_return_address = (uint64_t) next_task->run;
            scheduler.current_stack_address = next_task->stak_address;
            DEBUGH((uint64_t)next_task->stak_address);
            DEBUGH((uint64_t)next_task->run);
            set_userland_stack_pointer();
        }

        
        DEBUGH((uint64_t)next_task->stak_address)
        scheduler.current_stack_address = next_task->stak_address;
        
        return SCHEDULER_SUCCESS;
        }
    
}

#define RESTORE(i) \
__asm volatile("MOV X" #i ", %[reg]" : :[reg]"r"(scheduler.regs[i]));


void _store_reg_anx_ret(uint64_t base_address) {
    uint64_t *reg_base = (uint64_t *)base_address;
    
    DEBUG("Storing")
    *reg_base = scheduler.regs[30];
    DEBUGH(scheduler.regs[30])
    
    reg_base +=3;

    for (int i = 14; i >= 0; i--)
    {
        DEBUGH(scheduler.regs[2*i +1])
        *reg_base = scheduler.regs[2*i + 1];
        reg_base += 1;
        DEBUGH(scheduler.regs[2*i])
        *reg_base = scheduler.regs[2*i];
        reg_base += 1;
    }
}



#define STRINGIFY(x) #x

void _save_regs(uint64_t reg_base) {
    uint64_t * base_address = (uint64_t *) reg_base;
    uint64_t regs[31];
    DEBUG("Saving")
    // Save x30
    regs[30] = *base_address;
    DEBUGH(regs[30])

    base_address +=3;

    for (int i = 14; i >= 0; i--)
    {
        regs[2*i + 1] = *base_address;
        DEBUGH(regs[2*i +1])
        base_address += 1;
        regs[2*i] = *base_address;
        DEBUGH(regs[2*i])
        base_address += 1;
    }
    for (int i = 0; i < sizeof(regs) / sizeof(regs[0]); i++)
    {
        scheduler.regs[i] = regs[i];
    }
    
    // memcpy(scheduler.regs, regs, 248);
}

void _schedule_next(unsigned int delay, uint64_t base_address) {
    // clear_gicc_interrupt();
    set_timer(delay);
    enable_timer_int();
    _store_reg_anx_ret(base_address);
}


void _restore_pstate(uint64_t pstate) {
    __asm("MSR SPSR_EL3, %[pstate]" : : [pstate] "r"(pstate));
    DEBUGB(pstate)
}

void _restore_return_address(uint64_t elr) {
    __asm("MSR ELR_EL3, %[elr]" : : [elr] "r"(elr));
    DEBUGD(elr)
}

void __exception_gateway(uint64_t reg_base) {

    _save_regs(reg_base);
    DEBUG("Svaved regs")
    uint64_t pstate, elr;
    __asm("MRS %[pstate], SPSR_EL3" : [pstate]"=r"(pstate):);
    __asm("MRS %[pstate], ELR_EL3" : [pstate]"=r"(elr):);

    DEBUGB(pstate)
    DEBUGH(get_current_stack_pointer())
    DEBUGD(scheduler.current_task_index)
    DEBUGD((scheduler.current_task_index + 1) % MAX_TASK)
    if (_switch_task_context(scheduler.current_task_index,(scheduler.current_task_index + 1) % MAX_TASK, pstate, elr) == 0) {

        DEBUG("Restoring pstate")
        _restore_pstate(scheduler.current_pstate);

        DEBUG("Restoring ELR")
        _restore_return_address(scheduler.current_return_address);

        DEBUG("Setting userland staack")
        DEBUGH((uint64_t) scheduler.current_stack_address)
        
        _set_userland_stack_address(scheduler.current_stack_address);

        scheduler.registered_tasks[scheduler.current_task_index]->valid = 1;

        if (scheduler.initiated) {
            scheduler.current_task_index = (scheduler.current_task_index + 1) % MAX_TASK;
        }else {
            scheduler.initiated = 1;
        }
        
    };
    _schedule_next(100000000, reg_base);
}
