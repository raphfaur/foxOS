.section .text.context_switcher

.global _switcher_entry
.global __fiq_bridge
.global _smc_entry

_switcher_entry:
        STP X0, X1, [SP, #-16]!
        STP X2, X3, [SP, #-16]!
        STP X4, X5, [SP, #-16]!
        STP X6, X7, [SP, #-16]!
        STP X8, X9, [SP, #-16]!
        STP X10, X11, [SP, #-16]!
        STP X12, X13, [SP, #-16]!
        STP X14, X15, [SP, #-16]!
        STP X16, X17, [SP, #-16]!
        STP X18, X19, [SP, #-16]!
        STP X20, X21, [SP, #-16]!
        STP X22, X23, [SP, #-16]!
        STP X24, X25, [SP, #-16]!
        STP X26, X27, [SP, #-16]!
        STP X28, X29, [SP, #-16]!
        STR x30, [SP, #-16]!
        MOV X0, SP
        BL __exception_gateway
        ldr x30, [sp], #16
        ldp x28, x29, [sp], #16
        ldp x26, x27, [sp], #16
        ldp x24, x25, [sp], #16
        ldp x22, x23, [sp], #16
        ldp x20, x21, [sp], #16
        ldp x18, x19, [sp], #16
        ldp x16, x17, [sp], #16
        ldp x14, x15, [sp], #16
        ldp x12, x13, [sp], #16
        ldp x10, x11, [sp], #16
        ldp x8, x9, [sp], #16
        ldp x6, x7, [sp], #16
        ldp x4, x5, [sp], #16
        ldp x2, x3, [sp], #16
        ldp x0, x1, [sp], #16
        eret

_smc_entry:
        STP X18, X19, [SP, #-16]!
        STP X20, X21, [SP, #-16]!
        STP X22, X23, [SP, #-16]!
        STP X24, X25, [SP, #-16]!
        STP X26, X27, [SP, #-16]!
        STP X28, X29, [SP, #-16]!
        STR x30, [SP, #-16]!
        BL __synchronous_handler
        ldr x30, [sp], #16
        ldp x28, x29, [sp], #16
        ldp x26, x27, [sp], #16
        ldp x24, x25, [sp], #16
        ldp x22, x23, [sp], #16
        ldp x20, x21, [sp], #16
        ldp x18, x19, [sp], #16
        eret
