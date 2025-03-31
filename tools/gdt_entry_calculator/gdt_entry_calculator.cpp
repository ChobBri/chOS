#include <iostream>
#include <iomanip>
#include <cstdint>

int main(int argc, char* argv[]) {
    uint32_t base;
    uint32_t limit;
    uint8_t accessByte;
    uint8_t flags;
    std::cout << "Base memory address of segment: " << std::flush;
    std::cin >> std::hex >> base;

    std::cout << "Max addressable unit: " << std::flush;
    std::cin >> std::hex >> limit;
    std::cout << std::endl;

    uint8_t presentBit;  // Present bit. Must be 1 = valid segment
    uint8_t dplBits;     // CPU privilege level. 0 = highest (kernel), 3 = lowest (user)
    uint8_t sBit;        // Descriptor bit. 0 = system segment, 1 = code or data segment
    uint8_t eBit;        // Executable bit. 0 = data seg, 1 = code seg
    uint8_t dcBit;       // Direction/Conforming bit. If data seg, 0 = seg grows up, 1 = seg grows down. If code seg, 0 = code exec = ring (DPL), 1 = code exec <= ring
    uint8_t rwBit;       // Readable/Writable bit. 0 = disable read for code/write for data, 1 = enable "".
    uint8_t aBit;        // Accessed bit. CPU sets to 1 when access segment.
    std::cout << "[Access Byte]" << std::endl;
    std::cout << "Present: " << std::flush;
    std::cin >> presentBit;
    std::cout << "Descriptor Privilege Level: " << std::flush;
    std::cin >> dplBits;
    std::cout << "Descriptor Type: " << std::flush;
    std::cin >> sBit;
    std::cout << "Executable: " << std::flush;
    std::cin >> eBit;
    if (eBit == 0x0) {
        std::cout << "Direction: " << std::flush;
    }
    else {
        std::cout << "Conforming: " << std::flush;
    }
    std::cin >> dcBit;
    if (eBit == 0x0) {
        std::cout << "Writable: " << std::flush;
    }
    else {
        std::cout << "Readable: " << std::flush;
    }
    std::cin >> rwBit;
    std::cout << "Accessed: " << std::flush;
    std::cin >> aBit;
    std::cout << std::endl;

    accessByte = 0;
    accessByte |= ((aBit & 0x1) << 0);
    accessByte |= ((rwBit & 0x1) << 1);
    accessByte |= ((dcBit & 0x1) << 2);
    accessByte |= ((eBit & 0x1) << 3);
    accessByte |= ((sBit & 0x1) << 4);
    accessByte |= ((dplBits & 0x3) << 5);
    accessByte |= ((presentBit & 0x1) << 7);

    uint8_t granularityFlag;     // 0 = byte granularity, 1 = page granularity (4KiB)
    uint8_t sizeFlag;           // 0 = 16-bit prot mode, 1 = 32-bit prot mode
    uint8_t longFlag;           // 1 = 64-bit code seg
    std::cout << "[Flags]" << std::endl;
    std::cout << "Granularity: " << std::flush;
    std::cin >> granularityFlag;
    std::cout << "Size: " << std::flush;
    std::cin >> sizeFlag;
    std::cout << "Long-mode: " << std::flush;
    std::cin >> longFlag;

    flags = 0;
    flags |= ((longFlag & 0x1) << 1);
    flags |= ((sizeFlag & 0x1) << 2);
    flags |= ((granularityFlag & 0x1) << 3);

    uint64_t descriptor = 0;
    descriptor |= (limit & 0xFFFF);
    descriptor |= (base & 0xFFFF) << 16;
    descriptor |= (uint64_t)((base >> 16) & 0xFF) << 32;
    descriptor |= (uint64_t)(accessByte) << 40;
    descriptor |= (uint64_t)((((flags) & 0xF) << 4) | ((limit >> 16) & 0xF)) << 48;
    descriptor |= (uint64_t)((base >> 24) & 0xFF) << 56;

    std::cout << "GDT Entry in Bytes:" << std::endl;
    std::cout << std::hex << std::setfill('0') << std::setw(16) << descriptor << std::endl;
}
