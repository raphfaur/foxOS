#include "./syscall.h"


void send_sgi() {
  __asm volatile ("msr ICC_SGI0R_EL1, %[value]" : : [value] "r"(0x1ULL));
}

void syscall(int id) {
  __asm volatile("MOV x0, %[id]" :: [id]"r"((uint64_t)id));
  __asm volatile("SMC 0" ::);
}