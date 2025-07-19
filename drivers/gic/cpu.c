#include "gic.h"

uint32_t get_periphbase() {
  uint64_t value;
  __asm volatile("mrs %[systemr], S3_1_C15_C3_0" : [systemr] "=r"(value) :);
  return (uint32_t)  ((value << 24) >> (24 + 18) );
}

uint64_t get_affinity() {
  uint64_t value;
  __asm volatile("mrs %[systemr], MPIDR_EL1" : [systemr] "=r"(value) :);
  return value;
}

uint32_t get_ICC_SRE_EL1() {
  uint64_t value;
  __asm volatile("mrs %[systemr], ICC_SRE_EL1" : [systemr] "=r"(value) :);
  return (uint32_t) value;
}

void set_GICC_PMR(struct gic_cpu *dev) {
  uint64_t value;
  __asm volatile("mrs %[systemr], ICC_PMR_EL1" : [systemr] "=r"(value) :);
  value += 0xff;
  __asm volatile("msr ICC_PMR_EL1, %[systemr] ": : [systemr] "r"(value) );
  DEBUGH(value);
}

uint32_t get_ICC_IGRPEN0_EL1() {
  uint64_t value;
  __asm volatile("mrs %[systemr], ICC_IGRPEN0_EL1" : [systemr] "=r"(value) :);
  return (uint32_t) value;
}

uint32_t get_ICC_IGRPEN1_EL3() {
  uint64_t value;
  __asm volatile("mrs %[systemr], ICC_IGRPEN1_EL3" : [systemr] "=r"(value) :);
  return (uint32_t) value;
}

// Set group 1 for current sercure state
uint32_t set_ICC_IGRPEN1_EL1() {
  uint64_t value;
  __asm volatile("mrs %[systemr], ICC_IGRPEN1_EL1" : [systemr] "=r"(value) :);
  value |= 1;
  __asm volatile("msr ICC_IGRPEN1_EL1, %[systemr] ": : [systemr] "r"(value) );
  return (uint32_t) value;
}

// Set group 1 for both sercure states
uint32_t set_ICC_IGRPEN1_EL3() {
  uint64_t value;
  __asm volatile("mrs %[systemr], ICC_IGRPEN1_EL3" : [systemr] "=r"(value) :);
  value |= 1;
  __asm volatile("msr ICC_IGRPEN1_EL3, %[systemr] ": : [systemr] "r"(value) );
  return (uint32_t) value;
}

// set group 0
uint32_t set_ICC_IGRPEN0_EL1() {
  uint64_t value;
  __asm volatile("mrs %[systemr], ICC_IGRPEN0_EL1" : [systemr] "=r"(value) :);
  value |= 1;
  __asm volatile("msr ICC_IGRPEN0_EL1, %[systemr] ": : [systemr] "r"(value) );
  return (uint32_t) value;
}

// Routing
void set_FIQ_EL(int level) {
  if (level == 3) {
    uint64_t value;
    __asm volatile("mrs %[systemr], SCR_EL3" : [systemr] "=r"(value) :);
    value |= 4;
    __asm volatile("msr SCR_EL3, %[systemr]": : [systemr] "r"(value));
  }
}

uint64_t read_SCR_EL3() {
  uint64_t value;
  __asm volatile("mrs %[systemr], SCR_EL3" : [systemr] "=r"(value) :);
  return value;
}

void set_vbar(uint64_t value) {
  __asm volatile ("msr VBAR_EL1, %[value]" : : [value] "r"(value));
}

uint64_t read_vbar() {
  uint64_t value;
  __asm volatile("mrs %[systemr], VBAR_EL1" : [systemr] "=r"(value) :);
  return value;
}

uint64_t read_DAIF() {
  uint64_t value;
  __asm volatile("mrs %[systemr], daif" : [systemr] "=r"(value) :);
  return value;
}

void unset_daif_i() {
  uint64_t daif = read_DAIF();
  daif &= ~(uint64_t) 0b1 << 7;
  __asm volatile ("msr daif, %[value]" : : [value] "r"(daif));
}

void unset_daif_f() {
  uint64_t daif = read_DAIF();
  daif &= ~(uint64_t) 0b1 << 6;
  __asm volatile ("msr daif, %[value]" : : [value] "r"(daif));
}


uint32_t read_group1_intid() {
  uint64_t intid;
  __asm volatile("mrs %[systemr], ICC_IAR1_EL1" : [systemr] "=r"(intid) :);
  return intid;
}

uint32_t read_group0_intid() {
  uint64_t intid;
  __asm volatile("mrs %[systemr], ICC_IAR0_EL1" : [systemr] "=r"(intid) :);
  return intid;
}

void end_of_interrupt_group0(uint32_t intid) {
    __asm volatile ("msr ICC_EOIR0_EL1, %[value]" : : [value] "r"((uint64_t) intid));
}

void end_of_interrupt_group1(uint32_t intid) {
    __asm volatile ("msr ICC_EOIR1_EL1, %[value]" : : [value] "r"((uint64_t) intid));
}

void gic_cpu_init(struct gic_cpu* dev) {
  DEBUG("Beginning CPU Interface configuration")
  uint32_t periphbase = get_periphbase();
  dev->base_address = (void *) periphbase;

  // We only use Group 0 Secure
  DEBUG("Enabling interrupts group")
  set_ICC_IGRPEN0_EL1();

  // Group.1 disabled
  // uint32_t icc_irgpen1_el3 = get_ICC_IGRPEN1_EL3();

  // Set vector base address
  DEBUG("Setting vbar")
  set_vbar((uint64_t) &_vbar_address);

  // DAIF
  unset_daif_f();
  unset_daif_i();

  //PRM
  set_GICC_PMR(dev);

  uint64_t affinity = get_affinity();
  DEBUGB(affinity)
  DEBUG("\n")
}