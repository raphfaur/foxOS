CC := aarch64-none-elf-gcc
LD := aarch64-none-elf-ld
AS := aarch64-none-elf-as
OBJCPY := aarch64-none-elf-objcopy
OBJ := ./obj

ASFLAGS := -mcpu=cortex-a53
CCFLAGS := -g -ggdb -c -mcpu=cortex-a53

C_SRCS=$(shell find . -name '*.c')
S_SRCS=$(shell find . -name '*.s')

C_OBJS=$(C_SRCS:.c=.o)
S_OBJS=$(S_SRCS:.s=.o)

OBJS=$(C_OBJS) $(S_OBJS)

all: main.bin

$(C_OBJS): %.o: %.c
	$(CC) -c $(CCFLAGS) $< -o $@

$(S_OBJS): %.o: %.s
	$(CC) -c $(ASFLAGS) $< -o $@

debug: $(OBJS)
	@echo $(OBJS)

clean: $(OBJS)
	rm $(OBJS)

builds/main.elf : linker.ld $(OBJS)
	$(CC) -specs=nosys.specs -T  $^ -o $@ -nostartfiles -lm -L /Applications/ArmGNUToolchain/13.3.rel1/aarch64-none-elf/lib/gcc/aarch64-none-elf/13.3.1 

main.bin : builds/main.elf
	$(OBJCPY) -O binary builds/main.elf builds/main.bin

