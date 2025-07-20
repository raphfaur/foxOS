#include "../drivers/mmu/mmu.h"

extern void enter_el1();
extern void __kernel_entry();

/* The firware configures the MMU for EL1 Secure and jumps to kernel at 0xffffffff00000000 */
void __firmware_entry() {
    enter_el1();
}

void __kernel_jump() {
    mmu_init();
    __kernel_entry();
}

