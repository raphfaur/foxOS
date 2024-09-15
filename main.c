#include "./drivers/gic/cpu.h"
#include "./drivers/timer/timer.h"
#include "multicore/core.h"
#include "scheduling/scheduler.h"
#include "scheduling/tasks.h"
#include <stdint.h>
#include "./utils/debug.h"
#include "./drivers/smmu/mmu.h"
#include "./memory/allocator.h"

extern struct Scheduler scheduler;
extern char _t1_stack;
extern char _t2_stack;

void t1_start() {
  unsigned long long i = 0;
  int a = 0;
  while (1) {
    if (i == 10000) {
      a++;
      i = 0;
      DEBUG("T1")
      DEBUGD(a)
    }
    i++;
  }
  
}

void t2_start() {
  unsigned long long i = 0;
  int a = 0;
  while (1) {
    if (i == 10000) {
      DEBUG("T2")
      a++;
      i = 0;
      DEBUGD(a)
    }
    i++;
  }
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

  DEBUG("Initializing allocator");
  init_allocator();
  for (size_t i = 0; i < 16; i++)
  {
    alloc(0);
  }
  

  //__init(&scheduler);

  //register_task(&T1);
  //register_task(&T2);
  //__start(&scheduler);

  while (1) {
    // unsigned int waker = ((uint32_t)current_system_timer_value());
    // unsigned int ctl = get_timer_control_register();
    // if ((int) waker > -1000000) {
    //   DEBUGB(waker)
    //   ctl = get_timer_control_register();
    //   DEBUGB(ctl)
    //   DEBUG("\n")
    // } else {

    // }
  }
}


