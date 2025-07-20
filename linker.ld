MMU_CMDQ_SIZE = 0x200;
MMU_EVTQ_SIZE = 0x200;
MEMORY {
  bootloader : ORIGIN = 0x40000000, LENGTH = 5M
  heap : ORIGIN = ORIGIN(bootloader) + LENGTH(bootloader), LENGTH = 10M
  vectors : ORIGIN = ORIGIN(heap) + LENGTH(heap), LENGTH = 0x1000
  mmu : ORIGIN = ORIGIN(vectors) + LENGTH(vectors), LENGTH = 50M 
  kernel_code : ORIGIN = 0xffffffff00000000, LENGTH = 10M 
  debug_info : ORIGIN = 0x0, LENGTH = 0x0
}

ENTRY(_Startup)

SECTIONS {

  .startup : AT(0x40000000) {
    */startup.o
    bootloader/*.o(*)
  } > bootloader

  .vbar : ALIGN(0x100)
  {
    _vbar_address = .;
    */exceptions.o(.text.exceptions)
  } > vectors

  .kernel : AT(ALIGN(LOADADDR(.startup) + SIZEOF(.startup), 4K)) {
    kernel/*.o(.text* .data* .rodata* .bss*)
    drivers/*.o(.text* .data* .rodata* .bss*)
    *(.text*)
    *(.rodata*)
    *(.data*)
    *(.bss*)
  } > kernel_code

  _kernel_base_address = LOADADDR(.kernel);

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

  .mem : ALIGN(8) {
    _heap_start = .;
    . = . + 0x4000;
    end = _heap_start;
    . = . + 0x200;
    _stack_top = .;
    . = . + 0x40000;
    _t1_stack = .;
    . = . + 0x40000;
    _t2_stack = .;
    . = . + 0x40000;
    _heap_start1 = .;
    . = . + 0x40000;
    _user_space_base = .;
  } > heap

}

