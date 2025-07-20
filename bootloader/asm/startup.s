.global _Startup

_Startup:
  ldr x1, =_stack_top
  mov sp,x1
  B __firmware_entry
