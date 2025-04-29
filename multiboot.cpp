#include "multiboot.h"

extern uint32_t multiboot_info_ptr;
extern uint32_t multiboot_magic;



const multiboot_info* get_multiboot_info() {
    if (multiboot_magic == MULTIBOOT_BOOTLOADER_MAGIC) {
        return (const multiboot_info*) multiboot_info_ptr;
    }
    return nullptr;
}

bool isFlagSet(uint32_t flag) {
    return (((const multiboot_info*) multiboot_info_ptr)->flags & flag) == flag;
}
