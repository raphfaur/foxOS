.global __kernel_setup

__kernel_setup:
    // Enable FP and SIMD 
    MRS x0, CPACR_EL1
    ORR x0, x0, #(0b11 << 20)
    MSR CPACR_EL1, x0

    // Load kernel SP
    LDR x0,=_kernel_stack_top
    MOV sp, x0
    NOP

    B __kernel_entry