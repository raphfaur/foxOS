MMU_CMDQ_SIZE = 0x200;
MMU_EVTQ_SIZE = 0x200;
MEMORY {
  bootloader : ORIGIN = 0x40000000, LENGTH = 5M
  heap : ORIGIN = ORIGIN(bootloader) + LENGTH(bootloader), LENGTH = 10M
  vectors : ORIGIN = ORIGIN(heap) + LENGTH(heap), LENGTH = 0x1000
  mmu : ORIGIN = ORIGIN(vectors) + LENGTH(vectors), LENGTH = 50M 
  kernel_code : ORIGIN = 0xffffffff00000000, LENGTH = 0x40000000 /* 1GB */
  kernel_memory : ORIGIN = 0xffffffff80000000, LENGTH = 0x40000000 /* 1GB */
  debug_info : ORIGIN = 0x0, LENGTH = 0x0
}

ENTRY(_Startup)

SECTIONS {

  .startup : AT(0x40000000) {
    */startup.o
    bootloader/*.o(*)
    . = . + 0x4000;
    _bootloader_stack_top = .;
    _debug_heap = .;
    . = . + 0x4000;
  } > bootloader

  .vbar : ALIGN(0x100)
  {
    _vbar_address = .;
    */exceptions.o(.text.exceptions)
  } > vectors

  .kernel : AT(ALIGN(LOADADDR(.startup) + SIZEOF(.startup), 4K)) {
    kernel/*.o(.text* .data* .rodata* .bss*)
    drivers/*.o(.text* .data* .rodata* .bss*)
    kernel/memory/allocator.o(.bss*)
    *(.text*)
    *(.rodata*)
    *(.data*)
    *(.bss*)
    *(.eh_frame*)
  } > kernel_code
  _kernel_base_address = LOADADDR(.kernel);

  .kheap (NOLOAD) : ALIGN(4K) {
    _kernel_heap_start = .;
    . = . + 0x20000000;
    _kernel_heap_end = .;
    . = . + 0x40000;
    . = ALIGN(8);
    _kernel_stack_top = .;
  } > kernel_memory

  _kernel_memory_physical = ALIGN(LOADADDR(.kernel_mmu) + SIZEOF(.kernel_mmu), 4k);


  .user_mmu : ALIGN(0x10000) {
    _ulvl1_tbl = . ;
    . = . + 8 * 8;

    . = ALIGN(0x10000);
    _ulvl2_tbl = .;
    . = . + 8 * 512 * 8;

    . = ALIGN(0x10000);
    _ulvl3_tbl = .;
    . = . + 8 * 512 * 512 * 8;
  } > mmu

  .kernel_mmu : ALIGN(0x10000) {
    _klvl1_tbl = . ;
    . = . + 8 * 8;

    . = ALIGN(0x10000);
    _klvl2_tbl = .;
    . = . + 8 * 512 * 8;

    . = ALIGN(0x10000);
    _klvl3_tbl = .;
    . = . + 8 * 512 * 512 * 8;
  } > mmu
}

