.global enter_el1

el1_entry:
    ldr x1, =_stack_top
    mov sp,x1
    B __kernel_jump 

enter_el1:

    MOV      x1, #0              // Initial value of register is unknown
    ORR      x1, x1, #(1 << 11)  // set ST bit (disable trapping of timer control registers)
    ORR      x1, x1, #(1 << 10)  // set RW bit (next lower EL in aarch64)

    MSR      SCR_EL3, x1

    MSR      SCTLR_EL1, xzr

    mov x0, xzr
    msr sctlr_el1, x0


    LDR      x0, =el1_entry
    MOV      x1, #0b0101 // Secure EL1
    MSR      ELR_EL3, x0 // where to branch to when exception completes
    MSR      SPSR_EL3, x1 // set the program state for this point to a known value

    
    ERET
