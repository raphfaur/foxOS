MMU_CMDQ_SIZE = 0x200;
MMU_EVTQ_SIZE = 0x200;
MEMORY {
  bootloader : ORIGIN = 0x40000000, LENGTH = 5M
  heap : ORIGIN = ORIGIN(bootloader) + LENGTH(bootloader), LENGTH = 10M
  vectors : ORIGIN = ORIGIN(heap) + LENGTH(heap), LENGTH = 0x1000
  kernel_mmu : ORIGIN = ORIGIN(vectors) + LENGTH(vectors), LENGTH = 1500M 
  kernel_code : ORIGIN = ORIGIN(kernel_mmu) + LENGTH(kernel_mmu), LENGTH = 10M 
}

ENTRY(_Startup)
SECTIONS {
  # Firmware code
  .startup : AT(0x40000000) {
    */startup.o(.text)
  } > bootloader

  .text : {
    *(.text)
  } > bootloader

  .data : {
    *(.data)
  } > bootloader

  .rodata : {
    
    *(.rodata)
  } > bootloader

  .bdd : {
    *(.bss COMMON)
  } > bootloader

  .vbar : ALIGN(0x100)
  {
    _vbar_address = .;
    */exceptions.o(.text.exceptions)
  } > vectors

  # Kernel MMU 
  .k_lvl1_tbl : ALIGN(0x10000)  
  {
    _lvl1_tbl = . ;
    . = . + 512 * 8;
  } > kernel_mmu
  .k_lv2_tbl : ALIGN(0x10000)
  {
    _lvl2_tbl = .;
    . = . + 512 * 512 * 8;
  } > kernel_mmu
  .k_lv3_tbl : ALIGN(0x10000)
  {
    _lvl3_tbl = .;
    . = . + 512 * 512 * 512 * 8;
  } > kernel_mmu

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

