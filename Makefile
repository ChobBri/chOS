
kernel: entry.o kernel.o linker.ld
	~/opt/cross/bin/i686-elf-gcc -T linker.ld -o kernel -ffreestanding -O2 -nostdlib entry.o kernel.o -lgcc

kernel.o: kernel.c
	~/opt/cross/bin/i686-elf-gcc -c kernel.c -o kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

entry.o: entry.S
	~/opt/cross/bin/i686-elf-as entry.S -o entry.o


clean:
	rm entry.o kernel.o kernel