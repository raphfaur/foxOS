#include "./mmu.h"
#include "../../utils/debug.h"
#include <stdint.h>
extern char _lvl2_tbl;
extern char _lvl3_tbl;
extern char _user_space_base;

void mmu_set_ttbr0(void *address) {

  uint64_t ttbr0 = (uint64_t)address;
  DEBUGH(ttbr0);
  __asm volatile("MSR TTBR0_EL3, %[ttbr0]" : : [ttbr0] "r"(ttbr0));
  __asm volatile("MRS %[ttbr0], TTBR0_EL3" : [ttbr0] "=r"(ttbr0) :);
  DEBUGH(ttbr0);
}

void mmu_set_ttbr1(void *address) {
  uint64_t ttbr1 = (uint64_t)address;
  __asm volatile("MSR TTBR1_EL1, %[ttbr1]" : : [ttbr1] "r"(address));
}

void mmu_set_tcr(union TCR_EL1 *tcr) {
  uint64_t reg = tcr->value;
  __asm volatile("MSR TCR_EL3, %[reg] \n\t\
      ISB \n\t\
  " : : [reg] "r"(reg));
  __asm volatile("MRS %[reg], TCR_EL3" : [reg] "=r"(reg) :);
  DEBUGB(reg);
}

void mmu_enable() {
  uint64_t SCTLR_EL1;
  __asm volatile("MRS %[sctlr], SCTLR_EL3\n\t\
          ISB \n\t\
  " : [sctlr] "=r"(SCTLR_EL1) :);
  DEBUGB(SCTLR_EL1);
  SCTLR_EL1 |= 0x1ULL;
  __asm volatile("MSR SCTLR_EL3, %[sctlr] \n\t\
       ISB \n\t\
      "
                 :
                 : [sctlr] "r"(SCTLR_EL1));
    
}

void mmu_tcr_init() {
  union TCR_EL1 tcr;
  tcr.value = 0;
  // 64 KiB granularity
  tcr.field.TG0 = 0b10;
  // // 1 MiB address space size ?
  // tcr.field.PS = 0b010;
  /* 
  https://developer.arm.com/documentation/101811/0104/Translation-granule/The-starting-level-of-address-translation 
  The size of the address space is defined as 2^(64 - T0SZ) ie 64 - T0SZ bits addressable
  */
  // tcr.field.T1SZ = 10;
  mmu_set_tcr(&tcr);
}

void mmu_populate_tables(char *physical_base_address) {
  union mmu_tte_64k_lv2 *lv2_table = (union mmu_tte_64k_lv2 * ) &_lvl2_tbl;
  union mmu_tte_64k_lv3 (*lv3_tables)[8192] = (union mmu_tte_64k_lv3 (*) []) &_lvl3_tbl;
  

  for (int i = 0; i < 65 ; i++)
  {
    for (int j = 0; j < 8192; j++)
    {
      // A L3 table covers 2MB space and each block 4kb
      lv3_tables[i][j].block.addresss = 8192 * i + j;
      lv3_tables[i][j].block.AF = 0b01;
      lv3_tables[i][j].block.type = 0b01;
    }
    lv2_table[i].table.addresss = (uint64_t) &lv3_tables[i] >> 16;
    DEBUGH((uint64_t) &lv3_tables[i] );
    DEBUGH((uint64_t) &lv3_tables[i] >> 16);
    lv2_table[i].table.type = 0b11;
    // lv2_table[i].table.NS = 1;
  }

  DEBUGB(* (((uint64_t *)&_lvl2_tbl + 2)));
  DEBUGB(* (((uint64_t *)&_lvl2_tbl )));
  DEBUGB(* (((uint64_t *)&_lvl2_tbl + 3 )));
  DEBUGB(* (((uint64_t *)&_lvl3_tbl)));
  DEBUGB(* (((uint64_t *)&_lvl3_tbl) + 1));

  // while (1){}
}

// void set_index_physical_block(int index, void * block_address) {
//   mmu_table(*l3_tables)[64] = (mmu_table(*)[64]) & _lvl3_tbl;
//   union mmu_tte(*lvl2_tbl)[64] = (union mmu_tte(*)[64]) & _lvl2_tbl;
  
//   int i = index / 8192;
//   int j = index % 8192;


//   (*l3_tables)[i][j].block.addresss = (uint64_t) block_address >> 15;
//   (*l3_tables)[i][j].block.AF = 0b01;
//   (*l3_tables)[i][j].block.type = 0b01;
// };
 
void mmu_init() {
  mmu_set_ttbr0((void *)&_lvl2_tbl);
  mmu_populate_tables(&_user_space_base);
  mmu_tcr_init();
  mmu_enable();
}
