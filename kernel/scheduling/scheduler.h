#include "stdint.h"

#define MAX_TASK 2

#define SCHEDULER_ERROR 1
#define SCHEDULER_SUCCESS 0

struct Scheduler {
    struct Task * registered_tasks[MAX_TASK];
    int current_task_index;
    uint64_t current_pstate;
    uint64_t current_return_address;
    int task_n;
    void * current_stack_address;
    unsigned int initiated;
    uint64_t regs[31];
};

typedef unsigned int scheduler_error;



scheduler_error register_task(struct Task* t);
int __init(struct Scheduler *scheduler);
void __start(struct Scheduler* scheduler);
void __exception_gateway(uint64_t reg_base);