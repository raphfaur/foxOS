#include "./debug.h"

uint64_t get_current_stack_pointer() {
    uint64_t stack_t;
    __asm("MOV  %[stack_t], SP" : [stack_t]"=r"(stack_t) :);
    return stack_t;
}