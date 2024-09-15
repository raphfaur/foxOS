#include "./timer.h"
#include <stdint.h>

void set_current_system_timer_value(unsigned int timer) {
  uint64_t value;
  __asm volatile("mrs %[systemr], CNTP_TVAL_EL0" : [systemr] "=r"(value) :);
  uint64_t reg = (value >> 32) << 32;
  reg |= (uint64_t) timer;
  __asm volatile("msr CNTP_TVAL_EL0, %[systemr]" : : [systemr] "r"(reg));
}


unsigned int current_system_timer_value() {
  unsigned int time;
  uint64_t value;
  __asm volatile("mrs %[systemr], CNTP_TVAL_EL0" : [systemr] "=r"(value) :);
  time = value;
  return time;
}

void set_timer(uint64_t delay) {
 __asm volatile("msr CNTP_TVAL_EL0, %[systemr]" : : [systemr] "r"(delay));
}

uint64_t get_timer_control_register() {
  uint64_t value;
  __asm volatile("mrs %[systemr], CNTP_CTL_EL0" : [systemr] "=r"(value) :);
  return value;
}

void enable_timer_int() {
  // Set enable bit and unset mask bit
  uint64_t value;
  __asm volatile("mrs %[systemr], CNTP_CTL_EL0" : [systemr] "=r"(value) :);
  value = 1;
  __asm volatile("msr CNTP_CTL_EL0, %[systemr]" : : [systemr] "r"(value));
}


uint64_t current_system_count() {
  uint64_t value;
  __asm volatile("mrs %[systemr], CNTPCT_EL0" : [systemr] "=r"(value) :);
  return value;
}

uint64_t current_system_control_reg() {
  uint64_t value;
  __asm volatile("mrs %[systemr], SCTLR_EL1" : [systemr] "=r"(value) :);
  return value;
}

