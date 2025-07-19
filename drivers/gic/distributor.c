#include "gic.h"

void set_EnableGrp1S(struct gic_distributor * dev, int i );
uint32_t get_GICD_CTLR(struct gic_distributor *dev);


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

/* https://developer.arm.com/documentation/ihi0048/b/Programmers--Model/Distributor-register-descriptions/Interrupt-Set-Enable-Registers--GICD-ISENABLERn?lang=en */
void enable_intid_d(struct gic_distributor *dev, int intid) {
  int n = intid / 32;
  int offset = 0x100 + 4*n;
  int bit_n = intid % 32;
  volatile uint8_t * base = (volatile uint8_t *) dev->base_address;
  volatile uint32_t * reg_address = base + offset;
  DEBUGB(*reg_address);
  *reg_address |= 0b1ULL << bit_n;
  DEBUGB(*reg_address);
}

void route_intid_d(struct gic_distributor *dev, int intid, uint64_t affinity) {
  int offset = 0x6000 + (8 * (intid));
  volatile uint8_t * base = (volatile uint8_t *) dev->base_address;
  volatile uint64_t * reg_address = base + offset;
  DEBUGB(*reg_address);
  *reg_address |= (0b1ULL << 31);
  DEBUGB(*reg_address);
}

void set_mode_for_intid_d(struct gic_distributor *dev, int intid, uint8_t mode) {
  int n = intid / 16;
  int offset = 0xc00 + 4*n;
  int field_offset = intid % 16;
  volatile uint32_t * reg_address = (volatile uint32_t * ) (dev->base_address + offset);
  *reg_address |= mode << field_offset * 2;
  DEBUGB(*reg_address)
}

void is_active_intid_d(struct gic_distributor *dev, int intid) {
  int n = intid / 32;
  int offset = 0x300 + 4*n;
  int bit_offset = intid % 32;
  volatile uint32_t * is_active_reg = (volatile uint32_t * ) (dev->base_address + offset);
  // *is_active_reg |= 0b1 << bit_offset;
  DEBUGB(*is_active_reg)
}

void is_pending_intid_d(struct gic_distributor *dev, int intid) {
  int n = intid / 32;
  int offset = 0x0200 + (4 * n);
  int bit_offset = intid % 32;
  volatile uint32_t * is_pending_reg = (volatile uint32_t * ) (dev->base_address + offset);
  DEBUGB(*is_pending_reg)
}


void gic_distributor_init(struct gic_distributor* dev) {
  DEBUG("Begininning GIC Distributor configuration")
  dev->base_address = (void *) DISTRIBUTOR_BASE_ADDRESS;

  // Set ctlr register
  DEBUG("Configuring routing affinity") // Non legacy mode
  set_ARE_S(dev,1);

  // Enable all groups
  DEBUG("Enabling groups")
  set_EnableGrp1N(dev,1);
  set_EnableGrp0(dev, 1);
  set_EnableGrp1S(dev, 1);
}