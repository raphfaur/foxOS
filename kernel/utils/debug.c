#include "./debug.h"

uint64_t get_current_stack_pointer() {
    uint64_t stack_t;
    __asm("MOV  %[stack_t], SP" : [stack_t]"=r"(stack_t) :);
    return stack_t;
}

// int atoi(char *c) {
//     char * cursor = c;
//     int acc = 0;
//     int factor = 1;
//     while (*cursor != '\0') {
//         cursor++;
//     }
//     cursor--;
//     while (cursor >= c) {
//         acc += factor * (*cursor - '0');
//         factor *= 10;
//         cursor--;
//     }
//     return acc;
// }