#include <stddef.h>
#include <stdint.h>
#include "../drivers/mmu/mmu.h"

extern char _kernel_base_address;

extern char _klvl1_tbl;
extern char _klvl2_tbl;
extern char _klvl3_tbl;

extern char _ulvl1_tbl;
extern char _ulvl2_tbl;
extern char _ulvl3_tbl;

extern void _mmu_init;

void _firm_map_kernel_4kb() {
  union mmu_tte_4k_lv1 *lv1_table = (union mmu_tte_4k_lv1 *)&_klvl1_tbl;
  union mmu_tte_4k_lv2 (*lv2_table) [512] = (union mmu_tte_4k_lv2 (*) [] )&_klvl2_tbl;
  union mmu_tte_4k_lv3 (*lv3_tables)[512][512] = (union mmu_tte_4k_lv3 (*) [] )& _klvl3_tbl;

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

void _firm_map_userspace_4kb(){
  union mmu_tte_4k_lv1 *lv1_table = (union mmu_tte_4k_lv1 *)&_ulvl1_tbl;
  union mmu_tte_4k_lv2 (*lv2_table) [512] = (union mmu_tte_4k_lv2 (*) [] )&_ulvl2_tbl;
  union mmu_tte_4k_lv3 (*lv3_tables)[512][512] = (union mmu_tte_4k_lv3 (*) [] )& _ulvl3_tbl;

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