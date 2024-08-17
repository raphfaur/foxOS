#include "stdint.h"

struct Task {
    uint64_t pstate;
    uint64_t return_address;
    unsigned int valid;
    void (*run) ();
    void * stak_address;
    uint64_t regs[31];
};