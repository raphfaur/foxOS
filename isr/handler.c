#include "../drivers/gic/cpu.h"
#include "../utils/debug.h"


/*
See https://developer.arm.com/documentation/ddi0601/2025-06/AArch64-Registers/ESR-EL3--Exception-Syndrome-Register--EL3- for 
an explanation of ESR (Exception Symptom Register)

This tool helps in analyzing ESR content :
https://esr.arm64.dev
*/
void __synchronous_handler(){
    DEBUG("Handling synchronous exception from current level")
    uint64_t ESR;
    __asm volatile("mrs %[systemr], ESR_EL3" : [systemr] "=r"(ESR) :);
    DEBUGH(ESR)
    while(1){}
}



void __fiq_bridge(uint64_t id) {
    DEBUG("handling exception")
    
    while (1){}
}