#include <stdint.h>

extern char _mmu_stream_table;
extern char _mmu_evtq_base;
extern char _mmu_cmdq_base;
extern int MMU_CMDQ_SIZE;
extern int MMU_EVTQ_SIZE;

#define MMU_STREAM_TABLE_BASE &_mmu_stream_table
#define SMMU_STRTAB_BASE_CFG_OFFSET 0x0088
#define SMMU_STRTAB_BASE_OFFSET 0x0080
#define MMU_BASE_OFFSET 0x9050000
#define SMMU_IDR0 0x0000
#define SMMU_IDR1 0x0004
#define SMMU_CMDQ_OFFSET 0x0090
#define SMMU_EVTQ_OFFSET 0x00A0
#define SMMU_CR1_OFFSET 0x0028
#define SMMU_CR0_OFFSET 0x0020
#define SMMU_CR0ACK_OFFSET 0x0024
#define SMMU_IRQCTRL_OFFSET 0x0050
#define SMMU_S_INIT_OFFSET 0x803C

#define CONCAT(x, y) x ## y
#define EXPAND(x, y) CONCAT(x, y)
#define RESERVED EXPAND(reserved, __LINE__)


void mmu_set_page_size(int size);
int mmu_get_st_level();

void mmu_init();

struct __attribute__((packed)) SMMU_STRTAB_BASE {
  unsigned int RESERVED : 6;
  uint64_t ADDR : 46;
  uint16_t RESERVED : 10;
  unsigned int RA : 1;
  unsigned int RESERVED : 1;
};

struct __attribute__((packed)) SMMU_STRTAB_BASE_CFG {
  uint8_t Log2size : 6;
  uint8_t Split : 5;
  uint8_t RESERVED : 5;
  uint8_t Fmt : 2;
  uint16_t RESERVED : 14;
};

struct __attribute__((packed)) mmu_ste{
  unsigned int V : 1;
  unsigned int Config : 3;
  unsigned int S1Fmt : 2;
  uint64_t S1ContextPtr : 46;
  unsigned int RESERVED : 7;
  unsigned int S1CDMax : 5;
  uint32_t RESERVED[14];
};


struct __attribute__((packed)) mmu_cmdq {
  struct mmu_cmd_base {
    uint8_t Log2size : 5;
    uint64_t Addr : 47;
    uint16_t RESERVED : 10;
    uint8_t RA : 1;
    uint8_t RESERVED : 1;
  } base;
  struct mmu_cmd_prod {
    uint32_t Wr : 20;
    uint16_t RESERVED : 12;
  } prod;
  struct mmu_cmd_cons {
    uint32_t Rd : 20;
    uint8_t RESERVED : 4;
    uint8_t Err : 7;
    uint8_t RESERVED : 1;
  } cons;
};

struct __attribute__((packed)) mmu_evtq {
  struct mmu_evtq_base {
    uint8_t Log2size : 5;
    uint64_t Addr : 47;
    uint16_t RESERVED : 10;
    uint8_t WA : 1;
    uint8_t RESERVED : 1;
  } base;
  struct mmu_evtq_prod {
    uint32_t Wr : 20;
    uint16_t RESERVED : 11;
    uint8_t Ovflg : 1;
  } prod;
  struct mmu_evtq_cons {
    uint32_t Rd : 20;
    uint16_t RESERVED : 11;
    uint8_t Ovackflg : 1;
  } cons;
};


struct __attribute__((packed)) SMMU_CR1 {
  uint8_t QueueIc : 2;
  uint8_t QueueOc : 2;
  uint8_t QueueSh : 2;
  uint8_t TableIc : 2;
  uint8_t TablesOc : 2;
  uint8_t TableSh : 2;
  uint32_t RESERVED : 20;
};

struct __attribute__((packed)) SMMU_IRQ_CTRL {
  uint8_t GerrorIrqen : 1;
  uint8_t PriqIrqen : 1;
  uint8_t EventqIrqen : 1;
  uint32_t RESERVED : 29;
};

struct __attribute__((packed)) SMMU_CR0 {
  uint8_t SmmuEn : 1;
  uint8_t PriqEn : 1;
  uint8_t EventqEn : 1;
  uint8_t CmdqEn : 1;
  uint8_t Atschk : 1;
  uint8_t RESERVED : 1;
  uint8_t Vmw : 3;
  uint8_t RESERVED : 1;
  uint8_t DptWalkEn : 1;
  uint32_t RESERVED : 21;
};

struct __attribute__((packed)) SMMU_CR0ACK {
  uint8_t SmmuEn : 1;
  uint8_t PriqEn : 1;
  uint8_t EventqEn : 1;
  uint8_t CmdqEn : 1;
  uint8_t Atschk : 1;
  uint8_t RESERVED : 1;
  uint8_t Vmw : 3;
  uint8_t RESERVED : 1;
  uint8_t DptWalkEn : 1;
  uint32_t RESERVED : 21;
};

struct __attribute__((packed)) mmu_cd {
  uint8_t T0SZ : 6;
  uint8_t TG0 : 2;
  uint8_t IR0 : 2;
  uint8_t OR0 : 2;
  uint8_t SH0 : 2;
  uint8_t EPD0 : 1;
  uint8_t ENDI : 1;
  uint8_t T1SZ : 6;
  uint8_t TG1 : 2;
  uint8_t IR1 : 2;
  uint8_t OR1 : 2;
  uint8_t SH1 : 2;
  uint8_t EPD1 : 1;
  uint8_t V : 1;
  uint32_t RESERVED;
  uint8_t RESERVED : 4;
  uint64_t TTB0 : 48;
};


struct __attribute__((packed)) mmu_supersection {

};
