
C_SRCS = $(wildcard *.c)
S_SRCS = $(wildcard *.S)
OBJS = $(patsubst %.c,%.o,$(C_SRCS))
OBJS += $(patsubst %.S,%.o,$(S_SRCS))
 
kernel: $(OBJS) linker.ld
	~/opt/cross/bin/i686-elf-gcc -T linker.ld -o kernel -ffreestanding -O2 -nostdlib $(OBJS) -lgcc

%.o: %.c
	~/opt/cross/bin/i686-elf-gcc -c $< -o $@ -ffreestanding -O2 -Wall -Wextra

%.o: %.S
	~/opt/cross/bin/i686-elf-as $< -o $@

clean:
	rm $(OBJS) kernel