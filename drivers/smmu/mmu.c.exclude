#include "./mmu.h"
#include "../../utils/debug.h"
#include "math.h"
#include <stdint.h>
#include <string.h>

volatile static struct SMMU_CR0 *smmu_cr0 =
    (volatile struct SMMU_CR0 *)(MMU_BASE_OFFSET + SMMU_CR0_OFFSET);
volatile static struct SMMU_CR0ACK *smmu_cr0ack =
    (volatile struct SMMU_CR0ACK *)(MMU_BASE_OFFSET + SMMU_CR0ACK_OFFSET);

void mmu_set_page_size(int size) {
  volatile uint32_t *reg = (volatile uint32_t *)(MMU_BASE_OFFSET + SMMU_IDR1);
  DEBUGH((uint32_t)reg)
  //*reg &= ~(1UL << 31);
  //*reg |= ((uint32_t) size) << 31;
  //*reg = 1;
  DEBUGB(((uint32_t)size) << 31)
  DEBUGB(*reg)
}

void mmu_irq_init() {
  struct SMMU_IRQ_CTRL irqctrl = {
      .GerrorIrqen = 1,
      .PriqIrqen = 1,
      .EventqIrqen = 1,
  };

  volatile struct SMMU_IRQ_CTRL *irqctrl_a =
      (volatile struct SMMU_IRQ_CTRL *)(MMU_BASE_OFFSET + SMMU_IRQCTRL_OFFSET);
  *irqctrl_a = irqctrl;

  DEBUGB((uint64_t) * (uint32_t *)(MMU_BASE_OFFSET + SMMU_IRQCTRL_OFFSET));
}

void mmu_mem_attribute_init() {
  struct SMMU_CR1 cr1 = {0};
  volatile struct SMMU_CR1 *cr1_a =
      (volatile struct SMMU_CR1 *)(MMU_BASE_OFFSET + SMMU_CR1_OFFSET);

  *cr1_a = cr1;
}

void mmu_queue_init(int cmdq_log2size, int evtq_log2size) {
  // CMD Q
  DEBUGD(cmdq_log2size);
  struct mmu_cmdq cmdq = {.base =
                              {
                                  .Log2size = cmdq_log2size,
                                  .Addr = (uint64_t)&_mmu_cmdq_base,
                              },
                          .cons =
                              {
                                  .Rd = 0,
                              },
                          .prod = {
                              .Wr = 0,
                          }};

  struct mmu_evtq evtq = {
      .base = {.Log2size = evtq_log2size, .Addr = (uint64_t)&_mmu_evtq_base},
      .cons =
          {
              .Rd = 0,
          },
      .prod = {
          .Wr = 0,
      }};

  volatile struct mmu_cmdq *cmdq_a =
      (volatile struct mmu_cmdq *)(MMU_BASE_OFFSET + SMMU_CMDQ_OFFSET);
  volatile struct mmu_evtq *evtq_a =
      (volatile struct mmu_evtq *)(MMU_BASE_OFFSET + SMMU_EVTQ_OFFSET);

  *cmdq_a = cmdq;
  *evtq_a = evtq;
  DEBUGB((uint64_t)&_mmu_cmdq_base);
  DEBUGB(*(uint64_t *)cmdq_a);
}

void mmu_stream_table_init(int log2size) {
  // Setup stream table cfg
  struct SMMU_STRTAB_BASE_CFG cfg = {
      .Log2size = (uint32_t)log2size, .Split = 0, .Fmt = 0};
  struct SMMU_STRTAB_BASE_CFG *address =
      (struct SMMU_STRTAB_BASE_CFG *)(MMU_BASE_OFFSET +
                                      SMMU_STRTAB_BASE_CFG_OFFSET);
  memcpy((void *)address, &cfg, sizeof(cfg));

  // Create stream table
  int size = (int)pow(2, log2size);
  struct mmu_ste mmu_ste_table[size];
  for (int i = 0; i < size; i++) {
    mmu_ste_table[i].V = 0;
  }
  // Write stream table address
  struct SMMU_STRTAB_BASE base_cfg = {.ADDR = (uint64_t)MMU_STREAM_TABLE_BASE};
  volatile struct SMMU_STRTAB_BASE *base_address =
      (struct SMMU_STRTAB_BASE *)(MMU_BASE_OFFSET + SMMU_STRTAB_BASE_OFFSET);
  *base_address = base_cfg;
  DEBUGB(*(uint32_t *)(MMU_BASE_OFFSET + SMMU_IDR0))

  DEBUGD(MMU_CMDQ_SIZE);
}

void mmu_invalidate_tlb_all() {
  volatile uint32_t *smmu_init_a = (uint32_t *) (MMU_BASE_OFFSET + SMMU_S_INIT_OFFSET);   
  *smmu_init_a |= 0x1UL;
  DEBUGB(*smmu_init_a);
  while ( (*smmu_init_a & 0x1UL) != 0) {}
  DEBUG("TBL cleared");
}

void mmu_enable() {
  volatile uint32_t * cr0 = (volatile uint32_t *) smmu_cr0;
  *cr0 |= 0x1UL;
  DEBUGB(*cr0);
  while ((*(volatile uint32_t *)smmu_cr0ack & (0x1UL)) == 0) {
  }
  DEBUG("SMMU enabled");

}

void mmu_q_enable() {

  DEBUG("Enabling event queue");
  // smmu_cr0->CmdqEn = 1;
  DEBUGB(*(uint32_t *)smmu_cr0ack);
  *(volatile uint32_t *)smmu_cr0 |= 0x1UL << 3;
  DEBUGB(*(uint32_t *)smmu_cr0ack);
  while ((*(volatile uint32_t *)smmu_cr0ack & (0x1UL << 3)) == 0) {
    ;
  }
  DEBUG("CMDQ enabled");
  *(volatile uint32_t *)smmu_cr0 |= 0x1UL << 2;
  while ((*(volatile uint32_t *)smmu_cr0ack & (0x1UL << 2)) == 0) {
    ;
  }
  DEBUG("EVTQ enabled");
}

void mmu_init() {
  mmu_stream_table_init(1);
  mmu_queue_init(9, 9);
  mmu_irq_init();
  mmu_q_enable();
  mmu_invalidate_tlb_all();
  mmu_enable();
  DEBUGH((uint64_t) sizeof(struct mmu_ste))
}

int mmu_get_st_level() {
  volatile uint32_t *reg = (volatile uint32_t *)(MMU_BASE_OFFSET + SMMU_IDR0);
  return (*reg << 3) >> 30;
}
