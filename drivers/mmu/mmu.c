#include "./mmu.h"
#include "../../kernel/utils/debug.h"
#include <stdint.h>

extern char _ulvl1_tbl;
extern char _ulvl2_tbl;
extern char _ulvl3_tbl;

extern char _klvl1_tbl;
extern char _klvl2_tbl;
extern char _klvl3_tbl;

union mmu_tte_4k_lv1 *lv1_table = (union mmu_tte_4k_lv1 *)&_klvl1_tbl;
union mmu_tte_4k_lv2 (*lv2_table) [512] = (union mmu_tte_4k_lv2 (*) [] )&_klvl2_tbl;
union mmu_tte_4k_lv3 (*lv3_tables)[512][512] = (union mmu_tte_4k_lv3 (*) [] )& _klvl3_tbl;


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

void _mmu_kflush() {
  __asm volatile ("TLBI VMALLE1 \n\t\
    DSB ISH \n\t\
    ISB \n\t\
  ");
}

/* 
Given a physical page index phy_page, maps page_n pages of kernel memory to page_n pages of physical memory
starting at vir_addr_base kernel address
*/
_mmu_res_e _kmap_page_addr_n(size_t phy_page, size_t page_n, void * vir_addr_base) {
  size_t vir_page = (size_t) vir_addr_base >> PAGE_BIT_N;
  for (size_t p = vir_page; p < vir_page + page_n; p++) {
    int lv1 = p >> 18;
    int lv2 = p >> 9 & 0b111111111;
    int lv3 = p & 0b111111111;
    lv3_tables[lv1][lv2][lv3].block.addresss = phy_page + p - vir_page;
    lv3_tables[lv1][lv2][lv3].block.AF = 0b01;
    lv3_tables[lv1][lv2][lv3].block.type = 0b11;
  }
  _mmu_kflush();
  return MMU_RES_SUCCESS;
}

_mmu_res_e _kmap_addr_addr_n(void * phy_add, size_t page_n, void * vir_add) {
  if ( (size_t) phy_add & PAGE_MASK | (size_t) vir_add & PAGE_MASK ) return MMU_RES_INVALID_ADD;
  size_t page = (uint64_t) phy_add >> PAGE_BIT_N;
  return _kmap_page_addr_n(page, page_n, vir_add);
}

_mmu_res_e _kmap_addr_addr(void * phy_add, void * vir_add) {
  return _kmap_addr_addr_n(phy_add, 1, vir_add);
}

void mmu_init()
{
  mmu_enable();
}
