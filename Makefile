
C_SRCS = $(wildcard *.cpp)
S_SRCS = $(wildcard *.S)
OBJS = $(patsubst %.cpp,%.o,$(C_SRCS))
OBJS += $(patsubst %.S,%.o,$(S_SRCS))
 
kernel: $(OBJS) linker.ld
	~/opt/cross/bin/i686-elf-g++ -T linker.ld -o kernel -ffreestanding -O2 -nostdlib $(OBJS) -lgcc

%.o: %.cpp
	~/opt/cross/bin/i686-elf-g++ -c $< -o $@ -ffreestanding -O2 -Wall -Wextra

%.o: %.S
	~/opt/cross/bin/i686-elf-as $< -o $@

clean:
	rm $(OBJS) kernel