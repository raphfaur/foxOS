.global _Startup
.global _firm_mmu_init

_Startup:
  ldr x1, =_stack_top
  mov sp,x1
  B __firmware_entry

_firm_mmu_init:
  MOV x0, xzr
  // .TG1 - 4Kb granule
  // .TG0 - 4kb granule
  ORR x0, x0, #(32 << 16) // .T1SZ - 64 - 32 kernel address space
  ORR x0, x0, #(32) // .T0SZ - 64 - 32 userspace address space

  LDR x1, =_klvl1_tbl // Load table base address
  MSR TTBR1_EL1, x1

  LDR x1, =_ulvl1_tbl // Load table base address
  MSR TTBR0_EL1, x1

  MRS x2, SCTLR_EL1
  ORR x2, x2, #(0b01) // .M (Enable mmu)

  
  MSR TCR_EL1, x0
  MSR SCTLR_EL1, x2

  TLBI VMALLE1
  DSB ISH
  ISB

  RET
