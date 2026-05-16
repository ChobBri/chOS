#include "gdt.h"
#include <stdint.h>
#include <stddef.h>

typedef struct {
    uint16_t size;
    uint32_t base;
} __attribute__((packed)) gdt_descriptor;

/* GDT Segment Descriptor values */
/* Access Byte */
/* Present bit */
static constexpr uint8_t P_NOT_PRESENT = 0x0;
static constexpr uint8_t P_PRESENT = 0x1;

/* Descriptor Privilege Level */
static constexpr uint8_t DPL_KERNEL = 0x0;
static constexpr uint8_t DPL_1 = 0x1;
static constexpr uint8_t DPL_2 = 0x2;
static constexpr uint8_t DPL_USER = 0x3;

/* Descriptor Type */
static constexpr uint8_t S_SYSTEM_SEGMENT = 0x0;
static constexpr uint8_t S_CODE_OR_DATA_SEGMENT = 0x1;

/* Executable */
static constexpr uint8_t E_DATA_SEGMENT = 0x0;
static constexpr uint8_t E_CODE_SEGMENT = 0x1;

/* Direction/Conforming */
static constexpr uint8_t DC_DATA_GROWS_UP = 0x0;
static constexpr uint8_t DC_DATA_GROWS_DOWN = 0x1;
static constexpr uint8_t DC_CODE_EXECUTE_CPL = 0x0;
static constexpr uint8_t DC_CODE_EXECUTE_EQUAL_LOWER_PL = 0x1;

/* Readable/Writable */
static constexpr uint8_t RW_DATA_NOT_WRITEABLE = 0x0;
static constexpr uint8_t RW_DATA_WRITEABLE = 0x1;
static constexpr uint8_t RW_CODE_NOT_READABLE = 0x0;
static constexpr uint8_t RW_CODE_READABLE = 0x1;

/* Accessed */
static constexpr uint8_t A_NOT_ACCESSED = 0x0;
static constexpr uint8_t A_ACCESSED = 0x1;

/* Flags */
/* Granularity */
static constexpr uint8_t G_BYTE_GRANULARITY = 0x0;
static constexpr uint8_t G_PAGE_GRANULARITY = 0x1;

/* Size */
static constexpr uint8_t DB_16_BIT_PROTECTED_MODE_SEGMENT = 0x0;
static constexpr uint8_t DB_32_BIT_PROTECTED_MODE_SEGMENT = 0x1;

/* Long Mode */
static constexpr uint8_t L_NOT_64_BIT_CODE_SEGMENT = 0x0;
static constexpr uint8_t L_64_BIT_CODE_SEGMENT = 0x1;

typedef struct {
    uint32_t base;
    uint32_t limit;

    // Access Byte
    uint8_t  p : 1;      // Present; must be set for valid arg
    uint8_t  dpl : 2;    // Descriptor Privilege Level
    uint8_t  s : 1;      // Descriptor Type
    uint8_t  e : 1;      // Executable
    uint8_t  dc : 1;     // Direction/Conforming
    uint8_t  rw : 1;     // Readable/Writable
    uint8_t  a : 1;      // Accessed

    // Flags
    uint8_t g : 1;        // Granuality
    uint8_t db : 1;       // Size;
    uint8_t l : 1;
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

static constexpr int GDT_LEN = 3;
gdt_segment_descriptor_complex gdt[GDT_LEN];
static constexpr gdt_segment_descriptor gdt_entries[GDT_LEN] = {
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
        
        .p = P_PRESENT,
        .dpl = DPL_KERNEL,
        .s = S_CODE_OR_DATA_SEGMENT,
        .e = E_CODE_SEGMENT,
        .dc = DC_CODE_EXECUTE_CPL,
        .rw = RW_CODE_READABLE,
        .a = A_ACCESSED,
        
        .g = G_PAGE_GRANULARITY,
        .db = DB_32_BIT_PROTECTED_MODE_SEGMENT,
        .l = L_NOT_64_BIT_CODE_SEGMENT,
    },
    {  // DATA_SEGMENT
        .base = 0x0,
        .limit = 0xFFFFF,
        
        .p = P_PRESENT,
        .dpl = DPL_KERNEL,
        .s = S_CODE_OR_DATA_SEGMENT,
        .e = E_DATA_SEGMENT,
        .dc = DC_DATA_GROWS_UP,
        .rw = RW_DATA_WRITEABLE,
        .a = A_ACCESSED,
        
        .g = G_PAGE_GRANULARITY,
        .db = DB_32_BIT_PROTECTED_MODE_SEGMENT,
        .l = L_NOT_64_BIT_CODE_SEGMENT,
    }
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

extern "C" void load_gdt(gdt_descriptor* gdtr);
extern "C" void switch_to_32bit_protected();
void setup_gdt32(void) {
    for (int i = 0; i < GDT_LEN; i++) {
        gdt[i] = encodeGdtEntry(gdt_entries[i]);
    }
    gdtr.base = (uint32_t) &gdt;
    gdtr.size = sizeof(gdt_segment_descriptor_complex) * GDT_LEN - 1;
    load_gdt(&gdtr);
    switch_to_32bit_protected();
}
