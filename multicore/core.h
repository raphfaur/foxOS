#include "stddef.h"
#include "stdint.h"

int core_id() {
    uint64_t id;
    asm("MRS %[id], MPIDR_EL1" : [id]"=r"(id) : );
    return id & 0xffULL;
}