#include "./drivers/gic/gic.h"
#include "./drivers/smmu/mmu.h"
#include "./drivers/timer/timer.h"
#include "./memory/allocator.h"
#include "./memory/user_alloc.h"
#include "./utils/debug.h"
#include "api/syscall.h"
#include "multicore/core.h"
#include "scheduling/scheduler.h"
#include "scheduling/tasks.h"
#include "isr/handler.h"
#include "utils/io.h"
#include <stdint.h>
#include <string.h>


const char * WELCOME_MSG = 
  "\n\n\n"
  "Welcome to FoxOS \n"
  ">";


extern struct Scheduler scheduler;
extern char _t1_stack;
extern char _t2_stack;
extern void enter_el1 (void);
extern void bob (void);
extern struct serial UART;

void t1_start() {
  DEBUG("Hey");
  mmu_init();
}

void t2_start() {
  unsigned long long i = 0;
  int a = 0;
  uint64_t el;
  // while (1) {
  //   if (i == 10000) {
  //     DEBUG("T2")
  //     a++;
  //     i = 0;
  //     DEBUGD(a)
  //   }
  //   i++;
  // }
}

struct Task T1 = {.pstate = 0,
                  .return_address = 0,
                  .valid = 0,
                  .run = t1_start,
                  .stak_address = &_t1_stack};

struct Task T2 = {.pstate = 0,
                  .return_address = 0,
                  .valid = 0,
                  .run = t2_start,
                  .stak_address = &_t2_stack};

extern char _user_space_base;

void _main(void) {
  enter_el1();
  while (1) {}
}


void __timer_routine() {
  static int i = 0;
  DEBUG("Timer routine");
  set_timer(100000000);
}

void __uart_routine(){
  char received[2] = {0};
  char c = pl011_read_char(&UART);
  received[0] = c;
  if (IO_CHAR_FLUSH == received[0]) {
    received[0] = '\n';
    pl011_send(&UART, received);
    received[0] = '>';
  }
  _io_handle_input(c);
  pl011_send(&UART, received);
}

void __io_handler(char * data, int data_length) {
  if (memcmp(data, "bob", 3) == 0) {
    set_timer(100000000);
    enable_timer_int();
  }
}

void _kernel_entry(){

  io_register_flush_handler(__io_handler);
  pl011_init();
  // mmu_init();
  _gic_init();

  __register_routine_ppi(0xe, __timer_routine);
  __register_routine_spi(0x1, __uart_routine);

  // set_timer(1000000);
  // enable_timer_int();
  pl011_send(&UART, WELCOME_MSG);

  while(1){

  }
}
