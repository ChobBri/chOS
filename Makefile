
kernel: entry.o terminal.o kernel.o irq_stub.o ms_irq_stub.o minesweeper.o linker.ld
	~/opt/cross/bin/i686-elf-gcc -T linker.ld -o kernel -ffreestanding -O2 -nostdlib entry.o terminal.o  ms_irq_stub.o minesweeper.o irq_stub.o kernel.o -lgcc

kernel.o: kernel.c
	~/opt/cross/bin/i686-elf-gcc -c kernel.c -o kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

minesweeper.o: minesweeper.c
	~/opt/cross/bin/i686-elf-gcc -c minesweeper.c -o minesweeper.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

terminal.o: terminal.c
	~/opt/cross/bin/i686-elf-gcc -c terminal.c -o terminal.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

entry.o: entry.S
	~/opt/cross/bin/i686-elf-as entry.S -o entry.o
	
irq_stub.o: irq_stub.S
	~/opt/cross/bin/i686-elf-as irq_stub.S -o irq_stub.o
	
ms_irq_stub.o: ms_irq_stub.S
	~/opt/cross/bin/i686-elf-as ms_irq_stub.S -o ms_irq_stub.o

clean:
	rm entry.o terminal.o kernel.o irq_stub.o ms_irq_stub.o minesweeper.o kernel