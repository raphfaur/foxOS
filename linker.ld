MMU_CMDQ_SIZE = 0x200;
MMU_EVTQ_SIZE = 0x200;

ENTRY(_Startup)
SECTIONS {
  . = 0x40000000;
  .startup . : {
    obj/startup.o(.text)
  }
  . = ALIGN(0x100);
  _vbar_address = .;
  .text.exceptions : { obj/exceptions.o(.text.exceptions) }
  .text : {
    *(.text)
    obj/context_switcher.o(.text.context_switcher)
  }
  .data : {
    *(.data)
  }
  .bdd : {
    *(.bss COMMON)
  }

  # MMU  
  . = ALIGN(0x10000);
  _lvl1_tbl = . ;
  . = . + 512 * 8;

  . = ALIGN(0x10000);
  _lvl2_tbl = .;
  . = . + 512 * 512 * 8;

  . = ALIGN(0x10000);
  _lvl3_tbl = .;
  . = . + 512 * 512 * 512 * 8;

  . = ALIGN(8);
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
}

