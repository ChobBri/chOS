
kernel: entry.o kernel.o irq_stub.o linker.ld
	~/opt/cross/bin/i686-elf-gcc -T linker.ld -o kernel -ffreestanding -O2 -nostdlib entry.o irq_stub.o kernel.o -lgcc

kernel.o: kernel.c
	~/opt/cross/bin/i686-elf-gcc -c kernel.c -o kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

entry.o: entry.S
	~/opt/cross/bin/i686-elf-as entry.S -o entry.o
	
irq_stub.o: irq_stub.S
	~/opt/cross/bin/i686-elf-as irq_stub.S -o irq_stub.o

clean:
	rm entry.o kernel.o irq_stub.o kernel