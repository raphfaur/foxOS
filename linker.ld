ENTRY(_Startup)
SECTIONS {
  . = 0x40000000;
  .startup . : {
    startup.o(.text)
  }

  . = ALIGN(0x100);
  _vbar_address = .;
  .text.exceptions : { exceptions.o(.text.exceptions) }

  .text : {
    *(.text)
    context_switcher.o(.text.context_switcher)
  }
  .data : {
    *(.data)
  }
  .bdd : {
    *(.bss COMMON)
  }
  
  . = ALIGN(8);
  . = . + 0x40000;
  _stack_top = .;
  . = . + 0x40000;
  _t1_stack = .;
  . = . + 0x40000;
  _t2_stack = .;
  . = . + 0x40000;
  _heap_start1 = .;
  . = . + 0x40000;
  _heap_start2 = .;
  . = . + 0x40000;
  _heap_start3 = .;
}

