#include "gdt.h"
#include <stdint.h>
#include <stddef.h>

typedef struct {
    uint16_t size;
    uint32_t base;
} __attribute__((packed)) gdt_descriptor;

typedef struct {
    uint32_t base;
    uint32_t limit;

    // Access Byte
    uint8_t  p;      // Present
    uint8_t  dpl;    // Descriptor Privilege Level
    uint8_t  s;      // Descriptor Type
    uint8_t  e;      // Executable
    uint8_t  dc;     // Direction/Conforming
    uint8_t  rw;     // Readable/Writable
    uint8_t  a;      // Accessed

    // Flags
    uint8_t g;
    uint8_t db;
    uint8_t l;
} gdt_segment_descriptor;

typedef struct {
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_mid;
    uint8_t access_byte;
    uint8_t flags_limit_high;
    uint8_t base_high;
} __attribute__((packed)) gdt_segment_descriptor_complex;

gdt_descriptor gdtr;

#define GDT_LEN 5
gdt_segment_descriptor_complex gdt[GDT_LEN];
const gdt_segment_descriptor gdt_entries[GDT_LEN] = {
    {  // NULL_DESCRIPTOR
        .base = 0x0,
        .limit = 0x0,
        
        .p = 0x0,
        .dpl = 0x0,
        .s = 0x0,
        .e = 0x0,
        .dc = 0x0,
        .rw = 0x0,
        .a = 0x0,
        
        .g = 0x0,
        .db = 0x0,
        .l = 0x0,
    },
    {  // CODE_SEGMENT
        .base = 0x0,
        .limit = 0xFFFFF,
        
        .p = 0x1,
        .dpl = 0x0,
        .s = 0x1,
        .e = 0x1,
        .dc = 0x0,
        .rw = 0x1,
        .a = 0x1,
        
        .g = 0x1,
        .db = 0x1,
        .l = 0x0,
    },
    {  // DATA_SEGMENT
        .base = 0x0,
        .limit = 0xFFFFF,
        
        .p = 0x1,
        .dpl = 0x0,
        .s = 0x1,
        .e = 0x0,
        .dc = 0x0,
        .rw = 0x1,
        .a = 0x1,
        
        .g = 0x1,
        .db = 0x1,
        .l = 0x0,
    },
    {  // 16-Bit CODE_SEGMENT
        .base = 0x0,
        .limit = 0xFFFFF,
        
        .p = 0x1,
        .dpl = 0x0,
        .s = 0x1,
        .e = 0x1,
        .dc = 0x0,
        .rw = 0x1,
        .a = 0x1,
        
        .g = 0x0,
        .db = 0x0,
        .l = 0x0,
    },
    {  // 16-Bit DATA_SEGMENT
        .base = 0x0,
        .limit = 0xFFFFF,
        
        .p = 0x1,
        .dpl = 0x0,
        .s = 0x1,
        .e = 0x0,
        .dc = 0x0,
        .rw = 0x1,
        .a = 0x1,
        
        .g = 0x1,
        .db = 0x1,
        .l = 0x0,
    },
};

gdt_segment_descriptor_complex encodeGdtEntry(const gdt_segment_descriptor source)
{
    gdt_segment_descriptor_complex gdt_entry;
    // Encode the limit
    gdt_entry.limit_low = source.limit & 0xFFFF;
    gdt_entry.flags_limit_high = (source.limit >> 16) & 0x0F;
    
    // Encode the base
    gdt_entry.base_low = source.base & 0xFFFF;
    gdt_entry.base_mid = (source.base >> 16) & 0xFF;
    gdt_entry.base_high = (source.base >> 24) & 0xFF;
    
    // Encode the access byte
    uint8_t access_byte = ((source.p & 0x1) << 7)   | 
                          ((source.dpl & 0x3) << 5) | 
                          ((source.s & 0x1) << 4)   | 
                          ((source.e & 0x1) << 3)   | 
                          ((source.dc & 0x1) << 2)  | 
                          ((source.rw & 0x1) << 1)  | 
                          ((source.a & 0x1) << 0);
    gdt_entry.access_byte = access_byte;
    
    // Encode the flags
    uint8_t flags = ((source.g & 0x1) << 3)   | 
                    ((source.db & 0x1) << 2) | 
                    ((source.l & 0x1) << 1); 
    gdt_entry.flags_limit_high |= (flags << 4);
    return gdt_entry;
}

extern void load_gdt(gdt_descriptor* gdtr);
extern void switch_to_32bit_protected();
void setup_gdt32(void) {
    for (int i = 0; i < GDT_LEN; i++) {
        gdt[i] = encodeGdtEntry(gdt_entries[i]);
    }
    gdtr.base = (uint32_t) &gdt;
    gdtr.size = sizeof(gdt_segment_descriptor_complex) * GDT_LEN - 1;
    load_gdt(&gdtr);
    switch_to_32bit_protected();
}

