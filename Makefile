CC := aarch64-none-elf-gcc
LD := aarch64-none-elf-ld
AS := aarch64-none-elf-as
OBJCPY := aarch64-none-elf-objcopy
OBJ := ./obj

ASFLAGS := -mcpu=cortex-a53
CCFLAGS := -g -ggdb -c -mcpu=cortex-a53
SRCS=$(wildcard *.c) $(wildcard drivers/*.c)  

all: main.bin
$(OBJ)/%.o : drivers/gic/%.c
	$(CC) $(CCFLAGS) -o $@ $^

$(OBJ)/%.o : drivers/timer/%.c
	$(CC) $(CCFLAGS) -o $@ $^

$(OBJ)/%.o : drivers/uart/%.c
	$(CC) $(CCFLAGS) -o $@ $^

$(OBJ)/%.o : memory/%.c
	$(CC) $(CCFLAGS) -o $@ $^

$(OBJ)/%.o : multicore/%.c
	$(CC) $(CCFLAGS) -o $@ $^

$(OBJ)/%.o : drivers/mmu/%.c
	$(CC) $(CCFLAGS) -o $@ $^

$(OBJ)/%.o : scheduling/%.c
	$(CC) $(CCFLAGS) -o $@ $^

$(OBJ)/%.o : isr/%.c
	$(CC) $(CCFLAGS) -o $@ $^

$(OBJ)/%.o : api/%.c
	$(CC) $(CCFLAGS) -o $@ $^

$(OBJ)/%.o : utils/debug.c
	$(CC) $(CCFLAGS) -o $@ $^

$(OBJ)/main.o : main.c 
	$(CC) $(CCFLAGS) -o $@ $^

$(OBJ)/startup.o : asm/startup.s
	$(AS) $(ASFLAGS) -o $@ $^

$(OBJ)/context_switcher.o : asm/context_switcher.s
	$(AS) $(ASFLAGS) -o $@ $^

$(OBJ)/exceptions.o : asm/exceptions.s
	$(AS) $(ASFLAGS) -o $@ $^

builds/main.elf : linker.ld $(OBJ)/cpu.o $(OBJ)/mmu.o $(OBJ)/syscall.o $(OBJ)/handler.o $(OBJ)/user_alloc.o $(OBJ)/allocator.o $(OBJ)/context_switcher.o $(OBJ)/core.o $(OBJ)/debug.o $(OBJ)/scheduler.o $(OBJ)/timer.o $(OBJ)/pl001.o $(OBJ)/main.o $(OBJ)/startup.o $(OBJ)/exceptions.o
	$(CC) -specs=nosys.specs -T  $^ -o $@ -nostartfiles -lm -L /Applications/ArmGNUToolchain/13.3.rel1/aarch64-none-elf/lib/gcc/aarch64-none-elf/13.3.1 

main.bin : builds/main.elf
	$(OBJCPY) -O binary builds/main.elf builds/main.bin

