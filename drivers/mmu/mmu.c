#include "./mmu.h"
#include "../../kernel/utils/debug.h"
#include <stdint.h>

extern char _ulvl1_tbl;
extern char _ulvl2_tbl;
extern char _ulvl3_tbl;

extern char _klvl1_tbl;
extern char _klvl2_tbl;
extern char _klvl3_tbl;

extern char _user_space_base;

void mmu_set_ttbr0(void *address)
{
  uint64_t ttbr0 = (uint64_t)address;
  DEBUGH(ttbr0);
  __asm volatile("MSR TTBR0_EL1, %[ttbr0]" : : [ttbr0] "r"(ttbr0));
  __asm volatile("MRS %[ttbr0], TTBR0_EL1" : [ttbr0] "=r"(ttbr0) :);
  DEBUGH(ttbr0);
}

void mmu_set_ttbr1(void *address)
{
  uint64_t ttbr1 = (uint64_t)address;
  DEBUGH(ttbr1);
  __asm volatile("MSR TTBR1_EL1, %[ttbr1]" : : [ttbr1] "r"(ttbr1));
  __asm volatile("MRS %[ttbr1], TTBR1_EL1" : [ttbr1] "=r"(ttbr1) :);
  DEBUGH(ttbr1);
}

void mmu_set_mair()
{
  uint64_t mair;
  __asm volatile("MRS %[mair], MAIR_EL3" : [mair] "=r"(mair) :);
  DEBUGB(mair);
}

/*
  https://developer.arm.com/documentation/101811/0104/Translation-granule/The-starting-level-of-address-translation
  The size of the address space is defined as 2^(64 - T0SZ) ie 64 - T0SZ bits addressable
*/
void mmu_set_tcr(union TCR_EL1 *tcr)
{
  uint64_t reg = tcr->value;
  __asm volatile("MSR TCR_EL1, %[reg] \n\t\
      ISB \n\t\
  " : : [reg] "r"(reg));
  __asm volatile("MRS %[reg], TCR_EL1" : [reg] "=r"(reg) :);
  DEBUGB(reg);
}

void mmu_enable()
{
  uint64_t SCTLR_EL1;
  __asm volatile("MRS %[sctlr], SCTLR_EL1\n\t\
          ISB \n\t\
  " : [sctlr] "=r"(SCTLR_EL1) :);

  DEBUGB(SCTLR_EL1);
  SCTLR_EL1 |= 0x1ULL;
  __asm volatile("MSR SCTLR_EL1, %[sctlr] \n\t\
       TLBI VMALLE1\n\t\
       DSB ISH \n\t\
       ISB \n\t\
      "
                 :
                 : [sctlr] "r"(SCTLR_EL1));
                 
}

extern char _kernel_base_address;

void map_kernel_4kb() {
  union mmu_tte_4k_lv1 *lv1_table = (union mmu_tte_4k_lv1 *)&_klvl1_tbl;
  union mmu_tte_4k_lv2 (*lv2_table) [512] = (union mmu_tte_4k_lv2 (*) [] )&_klvl2_tbl;
  union mmu_tte_4k_lv3 (*lv3_tables)[512][512] = (union mmu_tte_4k_lv3 (*) [] )& _klvl3_tbl;

  DEBUGH((uint64_t) &_kernel_base_address >> 12);
  // Write table emtries
  for (int lv1 = 0; lv1 < 8; lv1++)
  {
    for (int lv2 = 0; lv2 < 512; lv2++)
    {
      for (int lv3 = 0; lv3 < 512; lv3++)
      {
        // A L3 table covers 2MB space and each block 4Kb
        lv3_tables[lv1][lv2][lv3].block.addresss = 512 * 512 * lv1 + 512 * lv2 + lv3 + ((uint64_t) &_kernel_base_address >> 12);
        lv3_tables[lv1][lv2][lv3].block.AF = 0b01;
        lv3_tables[lv1][lv2][lv3].block.type = 0b11;
      }
      lv2_table[lv1][lv2].table.addresss = (uint64_t) &lv3_tables[lv1][lv2] >> 12;
      lv2_table[lv1][lv2].table.type = 0b11;
    }
    lv1_table[lv1].table.addresss = (uint64_t) &lv2_table[lv1] >> 12;
    lv1_table[lv1].table.type = 0b11;
  }
}

void map_userspace_4kb(){
  union mmu_tte_4k_lv1 *lv1_table = (union mmu_tte_4k_lv1 *)&_ulvl1_tbl;
  union mmu_tte_4k_lv2 (*lv2_table) [512] = (union mmu_tte_4k_lv2 (*) [] )&_ulvl2_tbl;
  union mmu_tte_4k_lv3 (*lv3_tables)[512][512] = (union mmu_tte_4k_lv3 (*) [] )& _ulvl3_tbl;

  DEBUG("Mapping identity with 4Kb granule")

  // Write table emtries
  for (int lv1 = 0; lv1 < 8; lv1++)
  {
    for (int lv2 = 0; lv2 < 512; lv2++)
    {
      for (int lv3 = 0; lv3 < 512; lv3++)
      {
        // A L3 table covers 2MB space and each block 4Kb
        lv3_tables[lv1][lv2][lv3].block.addresss = 512 * 512 * lv1 + 512 * lv2 + lv3;
        lv3_tables[lv1][lv2][lv3].block.AF = 0b01;
        lv3_tables[lv1][lv2][lv3].block.type = 0b11;
      }
      lv2_table[lv1][lv2].table.addresss = (uint64_t) &lv3_tables[lv1][lv2] >> 12;
      lv2_table[lv1][lv2].table.type = 0b11;
    }
    lv1_table[lv1].table.addresss = (uint64_t) &lv2_table[lv1] >> 12;
    lv1_table[lv1].table.type = 0b11;
  }
}

void mmu_map_identity_4kb()
{
  DEBUG("Done writing")

  map_userspace_4kb();
  map_kernel_4kb();


  // Set TCR
  union TCR_EL1 tcr;
  tcr.field.TG1 = GRANULE_4K;
  tcr.field.TG0 = GRANULE_4K;
  // 38 bits address space (64 - 26 = 38)
  tcr.field.T0SZ = 32;
  tcr.field.T1SZ = 32;
  tcr.field.IPS = 0b001;
  mmu_set_tcr(&tcr);
  
  // Set TTBR0 to point to lv1 table
  mmu_set_ttbr0((void *)&_ulvl1_tbl);
  DEBUGH((uint64_t) &_ulvl1_tbl);
  DEBUGH((uint64_t) &_klvl1_tbl);
  
  mmu_set_ttbr1((void *)&_klvl1_tbl);

}

void mmu_map_identity_64kb(){
  union mmu_tte_64k_lv2 *lv2_table = (union mmu_tte_64k_lv2 *)&_ulvl2_tbl;
  union mmu_tte_64k_lv3(*lv3_tables)[8192] = (union mmu_tte_64k_lv3(*)[]) & _ulvl3_tbl;

  DEBUG("Mapping identity with 64Kb granule")

  // Write table emtries
  for (int i = 0; i < 100; i++)
  {
    for (int j = 0; j < 8192; j++)
    {
      // A L3 table covers 512MB space and each block 64Kb
      lv3_tables[i][j].block.addresss = 0x2000 * i + j ;
      lv3_tables[i][j].block.AF = 0b01;
      lv3_tables[i][j].block.type = 0b11;
    }
    lv2_table[i].table.addresss = (uint64_t)&lv3_tables[i] >> 16;
    lv2_table[i].table.type = 0b11;
  }

  // Set TCR
  union TCR_EL1 tcr;
  tcr.field.TG0 = GRANULE_64K;
  // 34 bits address space (64 - 30 = 34)
  tcr.field.T0SZ = 32;
  mmu_set_tcr(&tcr);
  
  // Set TTBR0 to point to lv2 table
  mmu_set_ttbr0((void *)&_ulvl2_tbl);

}


// void set_index_physical_block(int index, void * block_address) {
//   mmu_table(*l3_tables)[64] = (mmu_table(*)[64]) & _ulvl3_tbl;
//   union mmu_tte(*ulvl2_tbl)[64] = (union mmu_tte(*)[64]) & _ulvl2_tbl;

//   int i = index / 8192;
//   int j = index % 8192;

//   (*l3_tables)[i][j].block.addresss = (uint64_t) block_address >> 15;
//   (*l3_tables)[i][j].block.AF = 0b01;
//   (*l3_tables)[i][j].block.type = 0b01;
// };

void mmu_init()
{
  mmu_map_identity_4kb();
  mmu_enable();
  DEBUG("Enabled identity mapping")
}
