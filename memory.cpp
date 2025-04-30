#include "memory.h"
#include "multiboot.h"
#include <cstdint>
#include "terminal.h"
struct memory_header {
    uint32_t size;  /* in bytes */
    uint32_t next;
} __attribute__((packed));

memory_header* head = nullptr;
memory_header* tail = nullptr;
void init_memory() {
    extern uint32_t _linker_end;
    
    void* mem_start = (void*)(((uint32_t)&(_linker_end) + sizeof(uint32_t)) / sizeof(uint32_t) * sizeof(uint32_t));
    void* mem_end = 0;

    const multiboot_info* info = get_multiboot_info();
    multiboot_mmap_entry *mmap;
    for (mmap = (multiboot_mmap_entry *) info->mmap_addr;
        (uint32_t) mmap < info->mmap_addr + info->mmap_length;
        mmap = (multiboot_mmap_entry *) ((uint32_t) mmap + mmap->size + sizeof (mmap->size)))
        {   
            if (mmap->type == multiboot_mmap_entry::TYPE_AVAILABLE && mmap->addr_low == 0x100000) {
                mem_end = (void*) (mmap->len_low + 0x100000);
            }
        }

    head = (memory_header*) mem_start;
    head->size = (uint32_t) mem_end - (uint32_t) mem_start - sizeof(memory_header::size);
    head->next = 0x0;
    tail = head;
}

void* malloc(size_t size) {
    if (size == 0) {
        return nullptr;
    }

    memory_header* cur = head;
    memory_header* prev = cur;
    while (cur) {
        if (cur->size >= size) {
            memory_header* next = nullptr;
            uint32_t allocated_bytes = size;
            if (allocated_bytes % sizeof(uint32_t) != 0) {
                allocated_bytes = ((allocated_bytes + sizeof(uint32_t)) / sizeof(uint32_t)) * sizeof(uint32_t);
            }
                        
            next = (memory_header*)(&(cur->next) + (allocated_bytes / sizeof(uint32_t)));
            next->size = cur->size - allocated_bytes - sizeof(memory_header::size);
            next->next = 0x0;

            cur->size = allocated_bytes;
            cur->next = 0x0;

            if (cur == head) {
                head = next;
            }
            else {
                prev->next = (uint32_t) next;
            }

            return &(cur->next);
        }
        prev = cur;
        cur = (memory_header*) cur->next;

    }

    return nullptr;
}

void free(void* ptr) {
    if (ptr == nullptr) {
        return;
    }

    memory_header* new_region = (memory_header*) ((uint32_t*)ptr - 1);

    tail->next = (uint32_t) new_region;
    new_region->next = 0x0;
    tail = new_region;
}