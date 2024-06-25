RPI_VERSION = 4

ARM_GNU_TOOLCHAIN = aarch64-none-elf

BUILD_DIR = build
SRC_DIR = src
INC_DIR = inc

CFILES = $(wildcard *.c)
OFILES = $(CFILES:.c=.o)
GCCFLAGS = -Wall -O2 -ffreestanding -nostdinc -nostdlib -nostartfiles -Iinc
# GCCPATH = ../arm-gnu-toolchain-13.2.Rel1-x86_64-aarch64-none-elf/bin

all: clean kernel8.img

boot.o: boot.s
	$(ARM_GNU_TOOLCHAIN)-gcc $(GCCFLAGS) -c boot.s -o boot.o
# $(GCCPATH)/aarch64-none-elf-gcc $(GCCFLAGS) -c boot.s -o boot.o

%.o: %.c
	$(ARM_GNU_TOOLCHAIN)-gcc $(GCCFLAGS) -c $< -o $@
# $(GCCPATH)/aarch64-none-elf-gcc $(GCCFLAGS) -c $< -o $@

kernel8.img: boot.o $(OFILES)
	$(ARM_GNU_TOOLCHAIN)-ld -nostdlib boot.o $(OFILES) -T linker.ld -o kernel8.elf
	$(ARM_GNU_TOOLCHAIN)-objcopy -O binary kernel8.elf kernel8.img
# $(GCCPATH)/aarch64-none-elf-ld -nostdlib boot.o $(OFILES) -T linker.ld -o kernel8.elf
# $(GCCPATH)/aarch64-none-elf-objcopy -O binary kernel8.elf kernel8.img

clean:
	/bin/rm kernel8.elf *.o *.img > /dev/null 2> /dev/null || true
