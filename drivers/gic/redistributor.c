#include "gic.h"


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

void set_mode_for_intid_r(struct gic_redistributor *dev, int intid, uint8_t mode) {
  int n = intid / 16;
  int offset = GICR_SGI_BASE + GICR_ICFGR0 + (4 * n);
  int field_offset = intid % 16;
  volatile uint32_t * mode_reg = (volatile uint32_t * ) (dev->base_address + offset);
  *mode_reg |= mode << (2 * field_offset);
}

int get_intid_mode_0() {
  uint64_t value;
  __asm volatile("mrs %[systemr], ICC_IAR0_EL1" : [systemr] "=r"(value) :);
  return (int) value;
}

void gic_redistributor_init(struct gic_redistributor* dev) {
  DEBUG("Beginning GIC Redistributor configuration")

  // Wake up
  dev->base_address = (void *) REDISTRIBUTOR_BASE_ADDRESS;
  clear_ProcessorSleep(dev);
  poll_ChildrenAsleep(dev);
}




