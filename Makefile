RPI_VERSION = 4

ARM_GNU_TOOLCHAIN = aarch64-none-elf

BUILD_DIR = build
SRC_DIR = src
INC_DIR = inc

C_FILES = $(wildcard $(SRC_DIR)/*.c)
ASM_FILES = $(wildcard $(SRC_DIR)/*.S)
OBJ_FILES = $(C_FILES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
OBJ_FILES += $(ASM_FILES:$(SRC_DIR)/%.S=$(BUILD_DIR)/%.o)

GCCFLAGS = -Wall -O2 -ffreestanding -nostdinc -nostdlib -nostartfiles -Iinc

all: kernel8.img

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(@D)
	$(ARM_GNU_TOOLCHAIN)-gcc $(GCCFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.S
	mkdir -p $(@D)
	$(ARM_GNU_TOOLCHAIN)-gcc $(GCCFLAGS) -c $< -o $@

kernel8.img: $(OBJ_FILES)
	@echo "Generating kernel8.img for Raspberry PI $(value RPI_VERSION)"
	@echo ""
	$(ARM_GNU_TOOLCHAIN)-ld -T linker.ld -nostdlib -o $(BUILD_DIR)/kernel8.elf $(OBJ_FILES) 
	$(ARM_GNU_TOOLCHAIN)-objcopy -O binary $(BUILD_DIR)/kernel8.elf kernel8.img

clean:
	rm -rf build *.img > /dev/null 2> /dev/null || true
