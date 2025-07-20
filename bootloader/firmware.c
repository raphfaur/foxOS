#include "../drivers/mmu/mmu.h"

extern void enter_el1();
extern void __kernel_setup();
extern void _firm_mmu_init();

extern void _firm_map_kernel_4kb();
extern void _firm_map_userspace_4kb();

/* The firware configures the MMU for EL1 Secure and jumps to kernel at 0xffffffff00000000 */
void __firmware_entry() {
    enter_el1();
}

void __kernel_jump() {
    _firm_map_kernel_4kb();
    _firm_map_userspace_4kb();
    _firm_mmu_init();
    __kernel_setup();
}

