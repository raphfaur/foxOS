CC := clang
LD := clang
AS := aarch64-none-elf-as
# OBJCPY := aarch64-none-elf-objcopy
OBJ := ./obj

ASFLAGS := \
	--sysroot=/Applications/ArmGNUToolchain/13.3.rel1/aarch64-none-elf/aarch64-none-elf \
	--target=aarch64-none-elf \
	-mcpu=cortex-a53 \

CCFLAGS := \
	--sysroot=/Applications/ArmGNUToolchain/13.3.rel1/aarch64-none-elf/aarch64-none-elf \
	--target=aarch64-none-elf \
	-c \
	-mcpu=cortex-a53 \
	-mcmodel=large \
	-gdwarf64 \

LDFLAGS := \
	--sysroot=/Applications/ArmGNUToolchain/13.3.rel1/aarch64-none-elf/aarch64-none-elf \
	--target=aarch64-none-elf \
	-mcpu=cortex-a53 \
	-nostdlib \
	-fdebug-types-section \
	-mcmodel=large \
	-O0 \
	-gdwarf64 \

	

C_SRCS=$(shell find . -name '*.c')
S_SRCS=$(shell find . -name '*.s')

C_OBJS=$(C_SRCS:.c=.o)
S_OBJS=$(S_SRCS:.s=.o)

OBJS=$(C_OBJS) $(S_OBJS)


all: builds/main.elf



$(C_OBJS): %.o: %.c
	$(CC) -c $(CCFLAGS) $< -o $@

$(S_OBJS): %.o: %.s
	$(CC) -c $(ASFLAGS) $< -o $@

debug: $(OBJS)
	@echo $(OBJS)

clean: $(OBJS)
	rm $(OBJS)

builds/main.elf : $(OBJS)
	$(LD) $(LDFLAGS) $^ -o $@ -T linker.ld -lm -L/Applications/ArmGNUToolchain/13.3.rel1/aarch64-none-elf/aarch64-none-elf/lib -ffreestanding -fno-builtin

# main.bin : builds/main.elf
# 	$(OBJCPY) -O binary builds/main.elf builds/main.bin

run: builds/main.elf
	qemu-system-aarch64 -M virt,gic-version=3,virtualization=on,secure=on -cpu cortex-a53 -nographic -m 10G -gdb tcp::1234,nowait -kernel $^