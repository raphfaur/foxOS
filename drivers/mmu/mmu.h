#include <stdint.h>

#define CONCAT(x, y) x##y
#define EXPAND(x, y) CONCAT(x, y)
#define RESERVED EXPAND(reserved, __LINE__)

void mmu_set_ttbr0(void *address);

void mmu_set_ttbr1(void *address);


void mmu_enable();

union TCR_EL1 {
  uint64_t value;
  struct __attribute__((packed)) {
    uint8_t T0SZ : 6;
    uint8_t RESERVED : 2;
    uint8_t IR0 : 2;
    uint8_t OR0 : 2;
    uint8_t SH0 : 2;
    uint8_t TG0 : 2;
    uint8_t PS : 3;
    uint8_t T1SZ : 3;
    uint8_t A1 : 1;
    uint8_t EPD1 : 1;
    uint8_t IR1 : 2;
    uint8_t OR1 : 2;
    uint8_t SH1 : 2;
    uint8_t TG1 : 2;
    uint8_t IPS : 3;
    uint8_t RESERVED : 1;
    uint8_t AS : 1;
    uint8_t TBI0 : 1;
    uint8_t TBI1 : 1;
    uint32_t RESERVED : 25;
  } field;
};

struct __attribute__((packed)) mmu_l_attr {
  uint8_t INDX : 3;
  uint8_t NS : 1;
  uint8_t AP : 2;
  uint8_t SH : 2;
  uint8_t AF : 1;
};

struct __attribute__((packed)) mmu_h_attr {
  uint8_t RESERVED : 1 - 0;
  uint8_t PXN : 2 - 1;
  uint8_t UXN : 3 - 2;
  uint8_t __soft : 7 - 3;
  uint8_t RESERVED : 12 - 7;
};

struct __attribute__((packed)) mmu_table_tte {
  uint8_t type : 2 - 0 ;
  uint16_t RESERVED : 16 - 2;
  uint64_t addresss : 48 - 16;
  uint16_t RESERVED : 59 - 48;
  uint8_t PXN : 60 - 59;
  uint8_t XN : 61 - 60;
  uint8_t AP : 63 - 61;
  uint8_t NS : 64 - 63;
};

struct __attribute__((packed)) mmu_block_tte {
  uint8_t type : 2 - 0 ;
  uint8_t INDX : 3;
  uint8_t NS : 1;
  uint8_t AP : 2;
  uint8_t SH : 2;
  uint8_t AF : 1;
  uint32_t RESERVED : 17 - 12;
  uint64_t addresss : 48 - 17;
  uint16_t RESERVED : 52 - 48;
  uint8_t RESERVED : 1 - 0;
  uint8_t PXN : 2 - 1;
  uint8_t UXN : 3 - 2;
  uint8_t __soft : 7 - 3;
  uint8_t RESERVED : 12 - 7;
};

struct __attribute__((packed)) mmu_invalid_tte {
  uint8_t type : 1 - 0;
  uint64_t RESERVED : 64 - 1;
};

union mmu_tte {
  struct mmu_table_tte table;
  struct mmu_block_tte block;
  struct mmu_invalid_tte invalid;
};

void mmu_set_tcr(union TCR_EL1 *tcr);
