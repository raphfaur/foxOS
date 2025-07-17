.section .text.exceptions
.extern __exception_gateway

.global __fiq_bridge
.global _switcher_entry
.global _smc_entry
.global exception_vector_table

exception_vector_table:

/* 
Exception from the current EL while using SP_EL0 
*/

.org 0x0000
    B _smc_entry


.org 0x0080
    B _smc_entry


.org 0x0100
    B _smc_entry

.org 0x0180
    B _smc_entry


/*
Exception from the current EL while using SP_ELx
*/

/* Synchronous */
.org 0x0200
    smc_handler:
        B _smc_entry

.org 0x0280
    B _smc_entry

/* FIQ */
.org 0x0300
    timer_handler:
        B _switcher_entry

.org 0x0380
    B _smc_entry


/*
Exception from a lower EL and at least one lower EL is AArch64
*/ 

.org 0x0400
    B _smc_entry


.org 0x0480
    B _smc_entry


.org 0x0500
    B _smc_entry

.org 0x0580
    B _smc_entry

.org 0x0600
    B _smc_entry


.org 0x0680
    B _smc_entry

.org 0x0700
    timer_handler_0:
        B _switcher_entry

.org 0x0780
    B _smc_entry