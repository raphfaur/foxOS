#include "cpu.h"
#include <stdint.h>
#include "../../utils/debug.h"

extern char _vbar_address;
#define REDISTRIBUTOR_BASE_ADDRESS 0x80a0000
#define DISTRIBUTOR_BASE_ADDRESS 0x8000000

#define GICD_CTLR 0x0000
#define GICD_IGROUPR 0x0080
#define GICR_WAKER 0x0014


#define GICR_SGI_BASE 0x10000
#define GICR_IGROUP 0x0080
#define GICR_ISENABLER 0x0100
#define GICR_ICFGR1 0x0c04

#define GICC_PMR 0x0004


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
  // value = value << 11;
  __asm volatile ("msr VBAR_EL3, %[value]" : : [value] "r"(value));
  __asm volatile ("msr VBAR_EL1, %[value]" : : [value] "r"(value));
}

uint64_t read_vbar() {
  uint64_t value;
  __asm volatile("mrs %[systemr], VBAR_EL3" : [systemr] "=r"(value) :);
  return value;
}

uint64_t read_DAIF() {
  uint64_t value;
  __asm volatile("mrs %[systemr], daif" : [systemr] "=r"(value) :);
  return value;
}

void unset_daif_i() {
  uint64_t daif = read_DAIF();
  daif -= (uint64_t) 0b1 << 7;
  __asm volatile ("msr daif, %[value]" : : [value] "r"(daif));
}

void unset_daif_f() {
  uint64_t daif = read_DAIF();
  daif -= (uint64_t) 0b1 << 6;
  __asm volatile ("msr daif, %[value]" : : [value] "r"(daif));
}

void clear_gicc_interrupt_id(uint32_t intid) {
    __asm volatile ("msr ICC_EOIR0_EL1, %[value]" : : [value] "r"((uint64_t) intid));
}

uint32_t clear_gicc_interrupt() {
  uint32_t intid;
  __asm volatile("mrs %[systemr], ICC_IAR0_EL1" : [systemr] "=r"(intid) :);
  __asm volatile ("msr ICC_EOIR0_EL1, %[value]" : : [value] "r"(intid));
}

void gic_cpu_init(struct gic_cpu* dev) {
  DEBUG("Beginning CPU Interface configuration")
  uint32_t periphbase = get_periphbase();
  dev->base_address = (void *) periphbase;

  DEBUG("Enabling interrupts group")
  set_ICC_IGRPEN0_EL1();
  set_ICC_IGRPEN1_EL3();

  uint32_t icc_irgpen0_el1 = get_ICC_IGRPEN0_EL1();
  DEBUGB(icc_irgpen0_el1)
  uint32_t icc_irgpen1_el3 = get_ICC_IGRPEN1_EL3();
  DEBUGB(icc_irgpen1_el3)

  DEBUG("Routing FIQ")
  set_FIQ_EL(3);
  uint32_t scr_el3 = read_SCR_EL3();
  DEBUGB(scr_el3)

  // Set vector base address
  DEBUG("Setting vbar")

  DEBUGH((uint64_t)&_vbar_address);
  set_vbar((uint64_t) &_vbar_address);
  uint64_t vbar = read_vbar() >> 11;
  DEBUGH(vbar);

  // DAIF
  DEBUG("Unsetting FIQ mask bit")
  unset_daif_f();
  DEBUG("Unsetting IRQ mask bit")
  unset_daif_i();
  uint64_t daif = read_DAIF();
  DEBUGB(daif);

  //PRM
  set_GICC_PMR(dev);

  uint64_t affinity = get_affinity();
  DEBUGB(affinity)
  DEBUG("\n")
}

void set_EnableGrp1S(struct gic_distributor * dev, int i ) {
  volatile uint32_t * reg = (volatile uint32_t *) (dev->base_address + GICD_CTLR);
  *reg = *reg | 4;
}

void set_EnableGrp1N(struct gic_distributor * dev, int i ) {
  volatile uint32_t * reg = (volatile uint32_t *) (dev->base_address + GICD_CTLR);
  *reg = *reg | 2;
}

void set_EnableGrp0(struct gic_distributor * dev, int i ) {
  volatile uint32_t * reg = (volatile uint32_t *) (dev->base_address + GICD_CTLR);
  *reg = *reg | 1;
}

void set_ARE_S(struct gic_distributor * dev, int i ) {
  volatile uint32_t * reg = (volatile uint32_t *) (dev->base_address + GICD_CTLR);
  *reg = *reg | 16;
}

uint32_t get_GICD_CTLR(struct gic_distributor *dev) {
  return *((uint32_t *) (dev->base_address + GICD_CTLR));
}

void set_group_for_intid_d(struct gic_distributor *dev, int group, int intid) {
  volatile uint32_t * address = (volatile uint32_t * ) (dev->base_address + GICD_IGROUPR);
  address += intid;
  *address |= 0b1ULL << group ;
}

void gic_distributor_init(struct gic_distributor* dev) {
  DEBUG("Begininning GIC Distributor configuration")
  dev->base_address = (void *) DISTRIBUTOR_BASE_ADDRESS;
  // Set ctlr register
  DEBUG("Configuring routing affinity")
  set_ARE_S(dev,1);
  DEBUG("Enabling groups")
  set_EnableGrp1N(dev,1);
  set_EnableGrp0(dev, 1);
  set_EnableGrp1S(dev, 1);
  uint32_t distributor_ctlr = get_GICD_CTLR(dev);
  DEBUGB(distributor_ctlr);
}

uint32_t get_GICR_WAKER(struct gic_redistributor *dev) {
  return *((uint32_t *) (dev->base_address + GICR_WAKER));
}

void set_group_for_intid_r(struct gic_redistributor *dev, int group, int intid) {
  volatile uint32_t * address = (volatile uint32_t * ) (dev->base_address + GICR_SGI_BASE + GICR_IGROUP);
  *address |= 0b1ULL << intid;
  DEBUGB(*address)
}

void clear_ProcessorSleep(struct gic_redistributor *dev) {
  volatile uint32_t * reg = (volatile uint32_t *) (dev->base_address + GICR_WAKER);
  uint32_t mask = 0xffff;
  mask -= 2;
  DEBUG("Unsetting sleep bit")
  *reg &= mask;
}

void poll_ChildrenAsleep(struct gic_redistributor *dev) {
  volatile uint32_t * gicr_waker = (volatile uint32_t *) (dev->base_address + GICR_WAKER);
  DEBUG("Waiting for sleep bit to be unset")
  while (*gicr_waker & 4) {}
  DEBUG("Sleep bit unset")
  DEBUGB(*gicr_waker);
}

void enable_intid_r(struct gic_redistributor *dev, int intid) {
  volatile uint32_t * GICR_ISENABLER0_address = (volatile uint32_t * ) (dev->base_address + GICR_SGI_BASE +  GICR_ISENABLER);
  *GICR_ISENABLER0_address |= 0b1ULL << intid;
  DEBUGB(*GICR_ISENABLER0_address)
}

void set_mode_for_intid(struct gic_redistributor *dev, int intid) {
  volatile uint32_t * address_GICR_ICFGR1 = (volatile uint32_t * ) (dev->base_address + GICR_SGI_BASE + GICR_ICFGR1);
  *address_GICR_ICFGR1 |= 0b10ULL << (2 * (intid - 16));
  DEBUGB(*address_GICR_ICFGR1)
}

int get_intid_mode_0() {
  uint64_t value;
  __asm volatile("mrs %[systemr], ICC_IAR0_EL1" : [systemr] "=r"(value) :);
  return (int) value;
}
void gic_redistributor_init(struct gic_redistributor* dev) {
  DEBUG("Beginning GIC Redistributor configuration")
  dev->base_address = (void *) REDISTRIBUTOR_BASE_ADDRESS;
  clear_ProcessorSleep(dev);
  poll_ChildrenAsleep(dev);
  DEBUG("Setting group for intid 30")
  DEBUG("Enabling intid 30")
  // Enable TIMER interrupts
  set_mode_for_intid(dev, 30);
  enable_intid_r(dev, 30);
  // Enable SGI 1
  set_mode_for_intid(dev, 0);
  enable_intid_r(dev, 0);

  DEBUG("\n")
}




