
kernel: entry.o terminal.o kernel.o gdt.o keycode.o irq_stub.o linker.ld
	~/opt/cross/bin/i686-elf-gcc -T linker.ld -o kernel -ffreestanding -O2 -nostdlib entry.o gdt.o terminal.o keycode.o irq_stub.o kernel.o -lgcc

kernel.o: kernel.c
	~/opt/cross/bin/i686-elf-gcc -c kernel.c -o kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

gdt.o: gdt.c
	~/opt/cross/bin/i686-elf-gcc -c gdt.c -o gdt.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

keycode.o: keycode.c
	~/opt/cross/bin/i686-elf-gcc -c keycode.c -o keycode.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

terminal.o: terminal.c
	~/opt/cross/bin/i686-elf-gcc -c terminal.c -o terminal.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

entry.o: entry.S
	~/opt/cross/bin/i686-elf-as entry.S -o entry.o
	
irq_stub.o: irq_stub.S
	~/opt/cross/bin/i686-elf-as irq_stub.S -o irq_stub.o

clean:
	rm entry.o terminal.o kernel.o gdt.o keycode.o irq_stub.o kernel