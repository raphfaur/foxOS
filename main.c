#include "./drivers/gic/cpu.h"
#include "./drivers/smmu/mmu.h"
#include "./drivers/timer/timer.h"
#include "./memory/allocator.h"
#include "./memory/user_alloc.h"
#include "./utils/debug.h"
#include "api/syscall.h"
#include "multicore/core.h"
#include "scheduling/scheduler.h"
#include "scheduling/tasks.h"
#include <stdint.h>

extern struct Scheduler scheduler;
extern char _t1_stack;
extern char _t2_stack;
extern void enter_el1 (void);
extern void bob (void);

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



  // DEBUG("Config done");

  // syscall(2);
  // set_timer(10000000);
  // enable_timer_int();
  // mmu_init();
  enter_el1();

  struct gic_distributor gicd;
  struct gic_redistributor gicr;
  struct gic_cpu gicc;

  gic_distributor_init(&gicd);
  gic_redistributor_init(&gicr);
  gic_cpu_init(&gicc);

  mmu_init();

  // __init(&scheduler);

  // register_task(&T1);
  // __start(&scheduler);

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


void _kernel_entry(){
  uint8_t b = 0;
  DEBUG("Yo")

  mmu_init();
  
  while(1){}
}
