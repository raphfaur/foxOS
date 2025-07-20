.section .text.exceptions,"ax"
.extern __exception_gateway

.global __fiq_bridge
.global _switcher_entry
.global _smc_entry
.global exception_vector_table
.global exception_handler

exception_vector_table:

/* 
Exception from the current EL while using SP_EL0 
*/

.org 0x0000
    B .

.org 0x0080
    B .

.org 0x0100
    B .

.org 0x0180
    B .


/*
Exception from the current EL while using SP_ELx
*/

/* Synchronous */
.org 0x0200
    smc_handler:
        B .

.org 0x0280
    B .

/* FIQ */
.org 0x0300
    B exception_handler

.org 0x0380
    B .


/*
Exception from a lower EL and at least one lower EL is AArch64
*/ 

.org 0x0400
    B .


.org 0x0480
    B .


.org 0x0500
    B .

.org 0x0580
    B .

/*
Exception from a lower EL and at least one lower EL is AArch32
*/

.org 0x0600
    B .


.org 0x0680
    B .

.org 0x0700
    timer_handler_0:
        B .

.org 0x0780
    B .