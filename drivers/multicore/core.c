#include "core.h"
#include "../gic/gic.h"

int core_id() {
  uint64_t id;
  asm("MRS %[id], MPIDR_EL1" : [id] "=r"(id) :);
  return id & 0xffULL;
}

void _init_global_gic() {
  struct gic_distributor gic_distributor;
  gic_distributor_init(&gic_distributor);

  struct gic_redistributor gic_redistributor;
  gic_redistributor_init(&gic_redistributor);
}

void _init_cpu_interface() {
  struct gic_cpu gic_cpu;
  gic_cpu_init(&gic_cpu);
}

void _slave_core_entry() { _init_cpu_interface(); }

void _master_core_entry() { _init_global_gic(); }
