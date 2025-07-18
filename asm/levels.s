.global enter_el1
.global bob

el1_entry:
    ldr x1, =_stack_top
    mov sp,x1
    BL _kernel_entry

enter_el1:

    MOV      x1, #0              // Initial value of register is unknown
    ORR      x1, x1, #(1 << 11)  // set ST bit (disable trapping of timer control registers)
    ORR      x1, x1, #(1 << 10)  // set RW bit (next lower EL in aarch64)
    ORR      x1, x1, #(1 << 3)   // Set EA bit (SError routed to EL3)
    ORR      x1, x1, #(1 << 2)   // Set FIQ bit (FIQs routed to EL3)
    ORR      x1, x1, #(1 << 1)   // Set IRQ bit (IRQs routed to EL3)

    MSR      SCR_EL3, x1

    MSR      SCTLR_EL1, xzr

    mov x0, xzr
    orr x0, x0, #(1 << 29)          /* Checking http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.ddi0500d/CIHDIEBD.html */
    orr x0, x0, #(1 << 28)          /* Bits 29,28,23,22,20,11 should be 1 (res1 on documentation) */
    orr x0, x0, #(1 << 23)
    orr x0, x0, #(1 << 22)
    orr x0, x0, #(1 << 20)
    orr x0, x0, #(1 << 11)
    msr sctlr_el1, x0


    LDR      x0, =el1_entry
    MOV      x1, #0b0101
    MSR      ELR_EL3, x0 // where to branch to when exception completes
    MSR      SPSR_EL3, x1 // set the program state for this point to a known value
    
    ERET
