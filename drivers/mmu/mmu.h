#include <stdint.h>

#define CONCAT(x, y) x##y
#define EXPAND(x, y) CONCAT(x, y)
#define RESERVED EXPAND(reserved, __LINE__)

void mmu_set_ttbr0(void *address);

void mmu_set_ttbr1(void *address);

void mmu_enable();

enum MMU_GRANULE {
  GRANULE_4K = 0b00,
  GRANULE_64K = 0b01
};

union TCR_EL1 {
  uint64_t value;
  struct __attribute__((packed)) {
    uint8_t T0SZ : 6;
    uint8_t RESERVED : 1;
    uint8_t EPD0 : 1;
    uint8_t IRGN0 : 2;
    uint8_t ORGN0 : 2;
    uint8_t SH0 : 2;
    uint8_t TG0 : 2;
    uint8_t T1SZ : 21 - 16 + 1;
    uint8_t A1 : 1;
    uint8_t EPD1 : 1;
    uint8_t IRGN1 : 2;
    uint8_t ORGN1 : 2;
    uint8_t SH1 : 2;
    uint8_t TG1 : 2;
    uint8_t IPS : 34 - 32 + 1;
    uint8_t RESERVED : 1;
    uint8_t AS : 1;
    uint8_t TBI0 : 1;
    uint8_t TBI1 : 1;
    uint32_t RESERVED : 25;
  } field;
};

union TCR_EL3 {
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

/*
----------------
Block attributes
----------------
*/

// Low attributes
struct __attribute__((packed)) mmu_l_attr {
  uint8_t INDX : 3;
  uint8_t NS : 1;
  uint8_t AP : 2;
  uint8_t SH : 2;
  uint8_t AF : 1;
};

// High attributes
struct __attribute__((packed)) mmu_h_attr {
  uint8_t RESERVED : 1 - 0;
  uint8_t PXN : 2 - 1;
  uint8_t UXN : 3 - 2;
  uint8_t __soft : 7 - 3;
  uint8_t RESERVED : 12 - 7;
};


/* 
--------------
Table entries 
--------------
*/

// 64Kb granularity entry
struct __attribute__((packed)) mmu_table_tte_64k {
  uint8_t type : 2 - 0 ;
  uint16_t RESERVED : 16 - 2;
  uint64_t addresss : 48 - 16;
  uint16_t RESERVED : 59 - 48;
  uint8_t PXN : 60 - 59;
  uint8_t XN : 61 - 60;
  uint8_t AP : 63 - 61;
  uint8_t NS : 64 - 63;
};

// 4Kb granularity entry
struct __attribute__((packed)) mmu_table_tte_4k {
  uint8_t type : 2 - 0 ;
  uint16_t RESERVED : 12 - 2;
  uint64_t addresss : 48 - 12;
  uint16_t RESERVED : 59 - 48;
  uint8_t PXN : 60 - 59;
  uint8_t XN : 61 - 60;
  uint8_t AP : 63 - 61;
  uint8_t NS : 64 - 63;
};


/*
-------------
Block entries
-------------
*/

// 64k granularity
struct __attribute__((packed)) mmu_block_tte_64k_lv2 {
  uint8_t type : 2 - 0 ;

  // Low Attrs
  uint8_t INDX : 3;
  uint8_t NS : 1;
  uint8_t AP : 2;
  uint8_t SH : 2;
  uint8_t AF : 1;
  uint8_t nG : 1;
  
  uint32_t RESERVED : 29 - 12;
  uint64_t addresss : 48 - 29;
  uint16_t RESERVED : 52 - 48;

  // High Attrs
  uint8_t RESERVED : 1 - 0;
  uint8_t PXN : 2 - 1;
  uint8_t UXN : 3 - 2;
  uint8_t __soft : 7 - 3;
  uint8_t RESERVED : 12 - 7;
};

struct __attribute__((packed)) mmu_block_tte_64k_lv3 {
  uint8_t type : 2 - 0 ;
  
  // Low Attrs
  uint8_t INDX : 3;
  uint8_t NS : 1;
  uint8_t AP : 2;
  uint8_t SH : 2;
  uint8_t AF : 1;
  uint8_t nG : 1;

  uint32_t RESERVED : 16 - 12;
  uint64_t addresss : 48 - 16;
  uint16_t RESERVED : 52 - 48;

  // High Attrs
  uint8_t RESERVED : 1 - 0;
  uint8_t PXN : 2 - 1;
  uint8_t UXN : 3 - 2;
  uint8_t __soft : 7 - 3;
  uint8_t RESERVED : 12 - 7;
};

// 4k granularity
struct __attribute__((packed)) mmu_block_tte_4k_lv1 {
  uint8_t type : 2 - 0 ;

  // Low Attrs
  uint8_t INDX : 3;
  uint8_t NS : 1;
  uint8_t AP : 2;
  uint8_t SH : 2;
  uint8_t AF : 1;
  uint8_t nG : 1;

  uint32_t RESERVED : 30 - 12;
  uint64_t addresss : 48 - 30;
  uint16_t RESERVED : 52 - 48;

  // High Attrs
  uint8_t RESERVED : 1 - 0;
  uint8_t PXN : 2 - 1;
  uint8_t UXN : 3 - 2;
  uint8_t __soft : 7 - 3;
  uint8_t RESERVED : 12 - 7;
};

struct __attribute__((packed)) mmu_block_tte_4k_lv2 {
  uint8_t type : 2 - 0 ;

  // Low Attrs
  uint8_t INDX : 3;
  uint8_t NS : 1;
  uint8_t AP : 2;
  uint8_t SH : 2;
  uint8_t AF : 1;
  uint8_t nG : 1;

  uint32_t RESERVED : 21 - 12;
  uint64_t addresss : 48 - 21;
  uint16_t RESERVED : 52 - 48;

  // High Attrs
  uint8_t RESERVED : 1 - 0;
  uint8_t PXN : 2 - 1;
  uint8_t UXN : 3 - 2;
  uint8_t __soft : 7 - 3;
  uint8_t RESERVED : 12 - 7;
};

struct __attribute__((packed)) mmu_block_tte_4k_lv3 {
  uint8_t type : 2 - 0 ;

  // Low Attrs
  uint8_t INDX : 3;
  uint8_t NS : 1;
  uint8_t AP : 2;
  uint8_t SH : 2;
  uint8_t AF : 1;
  uint8_t nG : 1;

  uint64_t addresss : 48 - 12;
  uint16_t RESERVED : 52 - 48;

  // High Attrs
  uint8_t RESERVED : 1 - 0;
  uint8_t PXN : 2 - 1;
  uint8_t UXN : 3 - 2;
  uint8_t __soft : 7 - 3;
  uint8_t RESERVED : 12 - 7;
};


/*
-------------
Invalid entry
-------------
*/
struct __attribute__((packed)) mmu_invalid_tte {
  uint8_t type : 1 - 0;
  uint64_t RESERVED : 64 - 1;
};


/*
-----------------------
Translation table entry
-----------------------
*/

// 4k granularity 

// lv1 - Block or Table
union mmu_tte_4k_lv1 {
  struct mmu_block_tte_4k_lv1 block;
  struct mmu_table_tte_4k table;
  struct mmu_invalid_tte invalid;
  uint64_t raw;
};

// lv2 - Block or Table
union mmu_tte_4k_lv2 {
  struct mmu_block_tte_4k_lv2 block;
  struct mmu_table_tte_4k table;
  struct mmu_invalid_tte invalid;
  uint64_t raw;
};

// lv3 - Block
union mmu_tte_4k_lv3 {
  struct mmu_block_tte_4k_lv3 block;
  struct mmu_invalid_tte invalid;
  uint64_t raw;
};


// 64k granularity

// lv1 - Table
union mmu_tte_64k_lv1 {
  struct mmu_table_tte_64k table;
  struct mmu_invalid_tte invalid;
  uint64_t raw;
};

// lv2 - Table or Block
union mmu_tte_64k_lv2 {
  struct mmu_block_tte_64k_lv2 block;
  struct mmu_table_tte_64k table;
  struct mmu_invalid_tte invalid;
  uint64_t raw;
};

// lv3 - Block
union mmu_tte_64k_lv3 {
  struct mmu_block_tte_64k_lv3 block;
  struct mmu_invalid_tte invalid;
  uint64_t raw;
};


void mmu_set_tcr(union TCR_EL1 *tcr);
void mmu_init();