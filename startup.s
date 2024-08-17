.global _Startup
_Startup:
  ldr x1, =_stack_top
  mov sp,x1
  BL _main
  B .
