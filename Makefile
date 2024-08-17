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

$(OBJ)/%.o : multicore/%.c
	$(CC) $(CCFLAGS) -o $@ $^

$(OBJ)/%.o : scheduling/%.c
	$(CC) $(CCFLAGS) -o $@ $^

$(OBJ)/%.o : debug.c
	$(CC) $(CCFLAGS) -o $@ $^

$(OBJ)/main.o : main.c 
	$(CC) $(CCFLAGS) -o $@ $^

startup.o : startup.s
	$(AS) $(ASFLAGS) startup.s -o startup.o

context_switcher.o : context_switcher.s
	$(AS) $(ASFLAGS) context_switcher.s -o context_switcher.o

exceptions.o : exceptions.s
	$(AS) $(ASFLAGS) exceptions.s -o exceptions.o

main.elf : linker.ld $(OBJ)/cpu.o  context_switcher.o $(OBJ)/core.o $(OBJ)/debug.o $(OBJ)/scheduler.o $(OBJ)/timer.o $(OBJ)/pl001.o $(OBJ)/main.o startup.o exceptions.o
	$(CC) -specs=nosys.specs -T  $^ -o $@ -nostartfiles 

main.bin : main.elf
	$(OBJCPY) -O binary main.elf main.bin

