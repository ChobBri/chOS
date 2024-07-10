RPI_VERSION = 4

ARM_GNU_TOOLCHAIN = aarch64-none-elf

BUILD_DIR = build
SRC_DIR = src
INC_DIR = inc

C_FILES = $(wildcard $(SRC_DIR)/*.c)
ASM_FILES = $(wildcard $(SRC_DIR)/*.S)
OBJ_FILES = $(C_FILES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
OBJ_FILES += $(ASM_FILES:$(SRC_DIR)/%.S=$(BUILD_DIR)/%.o)

GCCFLAGS = -Wall -O2 -ffreestanding -nostdlib -nostartfiles -Iinc -mgeneral-regs-only

all: kernel8.img

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(@D)
	$(ARM_GNU_TOOLCHAIN)-gcc $(GCCFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.S
	mkdir -p $(@D)
	$(ARM_GNU_TOOLCHAIN)-gcc $(GCCFLAGS) -c $< -o $@

kernel8.img: $(OBJ_FILES)
	@echo "-------------------------------------------"
	@echo " Generating kernel8.img for Raspberry PI $(value RPI_VERSION) "
	@echo "-------------------------------------------"
	$(ARM_GNU_TOOLCHAIN)-ld -T linker.ld -nostdlib -o $(BUILD_DIR)/kernel8.elf $(OBJ_FILES) 
	# $(ARM_GNU_TOOLCHAIN)-objdump -x $(BUILD_DIR)/kernel8.elf
	$(ARM_GNU_TOOLCHAIN)-objcopy -O binary $(BUILD_DIR)/kernel8.elf kernel8.img

armstub/build/armstub.o: armstub/src/armstub.S
	mkdir -p $(@D)
	$(ARM_GNU_TOOLCHAIN)-gcc $(GCCFLAGS) -c $< -o $@

armstub: armstub/build/armstub.o
	$(ARM_GNU_TOOLCHAIN)-ld --section-start=.text=0 -o armstub/build/armstub.elf armstub/build/armstub.o
	$(ARM_GNU_TOOLCHAIN)-objcopy armstub/build/armstub.elf -O binary armstub-new.bin

clean:
	rm -rf build *.img *.bin > /dev/null 2> /dev/null || true

clean_armstub:
	rm -rf armstub/build *.bin > /dev/null 2> /dev/null || true
