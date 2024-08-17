#include <stdint.h>

char *dec(unsigned x, char *s);

unsigned int current_system_timer_value();
uint64_t current_system_control_reg();
uint32_t get_cpsr();
void set_current_system_timer_value(unsigned int timer) ;
uint64_t current_system_count();
void set_timer(uint64_t delay);
uint64_t get_timer_control_register();
void enable_timer_int();