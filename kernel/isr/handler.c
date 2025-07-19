#include "../../drivers/gic/gic.h"
#include "../../utils/debug.h"
#include "handler.h"

/*
See https://developer.arm.com/documentation/ddi0601/2025-06/AArch64-Registers/ESR-EL3--Exception-Syndrome-Register--EL3- for 
an explanation of ESR (Exception Symptom Register)

This tool helps in analyzing ESR content :
https://esr.arm64.dev
*/
void __synchronous_handler(){
    // DEBUG("Handling synchronous exception from current level")
    uint64_t ESR;
    __asm volatile("mrs %[systemr], ESR_EL3" : [systemr] "=r"(ESR) :);
    DEBUGH(ESR)
    while(1){}
}

static isr_routine ROUTINES[1020] = {NULL};

void __asynchronous_handler(){
    uint32_t intid;

    intid = read_group0_intid();

    // Check intid validity
    if (intid > sizeof(ROUTINES) / sizeof(ROUTINES[0])) {
        DEBUG("FATAL ERROR")
        DEBUGH(intid)
        while(1){}
    }

    if (NULL == ROUTINES[intid]) {
        DEBUG("FATAL ERROR")
        DEBUG("No routine defined for intid")
        DEBUGH(intid)
        while(1){}
    }

    // Call routine
    ROUTINES[intid]();

    end_of_interrupt_group0(intid);
}

void __register_routine(uint32_t intid, isr_routine routine){
    ROUTINES[intid] = routine;
}

void __register_routine_spi(uint32_t spi, isr_routine routine){
    __register_routine(spi + 32, routine);
    _gic_register_spi(spi, 0, GIC_MODE_LEVEL_SENSITIVE);
}

void __register_routine_ppi(uint32_t ppi, isr_routine routine){
    __register_routine(ppi + 16, routine);
    _gic_register_ppi(ppi, GIC_MODE_LEVEL_SENSITIVE);
}

void __register_routine_sgi(uint32_t sgi, isr_routine routine){
    __register_routine(sgi, routine);
}