#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "keycode.h"
#include "terminal.h"
#include "gdt.h"
#include "string.h"
#include "x86.h"
#include "vga.h"
#include "vec2.h"
#include "screen.h"
#include "keyboard.h"
#include "world.h"
#include "multiboot.h"
#include "memory.h"
#include "math.h"
static constexpr int IDT_SIZE = 256;

typedef struct {
    uint16_t offset_low;   // Lower 16 bits of handler address
    uint16_t selector;     // Code segment selector
    uint8_t  zero;         // Reserved (set to 0)
    uint8_t  type_attr;    // Type and attributes
    uint16_t offset_high;  // Upper 16 bits of handler address
} __attribute__((packed)) idt_entry;

// IDT Descriptor
typedef struct {
    uint16_t limit;       // size in bytes - 1
    uint32_t base;        // start memory location
} __attribute__((packed)) idt_descriptor;

__attribute__((aligned(0x10))) 
idt_entry idt[IDT_SIZE];

// IDT Descriptor
idt_descriptor idtr;
void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags) {
    idt[num].offset_low  = base & 0xFFFF;        // Low 16 bits of ISR address
    idt[num].selector    = sel;                  // Kernel segment selector (0x08)
    idt[num].zero        = 0;                    // Reserved
    idt[num].type_attr   = flags;                // Type and attributes
    idt[num].offset_high = (base >> 16) & 0xFFFF; // High 16 bits of ISR address
}

void set_timer_phase(int hz) {
    int divisor = 1193182 / hz;
    outb(0x43, 0x36);       // Set mode: square wave generator
    outb(0x40, divisor & 0xFF);
    outb(0x40, divisor >> 8);
}

extern "C" void irq_stub_32();
extern "C" void irq_stub_33();
// Load IDT using inline assembly
void load_idt() {
    asm volatile(
        "movb $0x11, %al\n"  // ICW
        "outb %al, $0x20\n"  // PIC1
        "outb %al, $0xA0\n"  // PIC2
        
        "movb $0x20, %al\n"  // new offset for PIC1
        "outb %al, $0x21\n"
        "movb $0x28, %al\n"  // new offset for PIC2
        "outb %al, $0xA1\n"
        
        "movb $0x04, %al\n"  // connect PIC1 to PIC2
        "outb %al, $0x21\n"
        "movb $0x02, %al\n"  // connect PIC2 to PIC1
        "outb %al, $0xA1\n"
        
        "movb $0x01, %al\n"  // PICs use 8086 mode
        "outb %al, $0x21\n"
        "outb %al, $0xA1\n"

        "movb $0xFC, %al\n"  // Enable IRQ1 keyboard
        "outb %al, $0x21\n"
        "movb $0xFF, %al\n"
        "outb %al, $0xA1\n"
    );
    idtr.limit = sizeof(idt) - 1;
    idtr.base = (uint32_t) &idt;
    set_timer_phase(60);

    idt_set_gate(32, (uint32_t) irq_stub_32, 0x08, 0x8E);
    idt_set_gate(33, (uint32_t) irq_stub_33, 0x08, 0x8E);
    
    asm volatile ("lidt %0" : : "m" (idtr));
    asm volatile ("sti");
}

void handleTimerInterrupt(){
    // char num_buffer[33];
    // static int i = 0;
    // i++;
    // terminal_writestring(itoa(i, num_buffer, 10));
    // terminal_putchar('\n');
}

void playSound();
void handleKeyboardInterrupt(){
    uint8_t scancode = inb(0x60);
    // terminal::putchar(scancode);
    keyboard::handleScanCode(scancode);
    if (keyboard::isKeyPressed(keycode::Space)) {
        playSound();
    }
}

extern "C"
void interrupt_handler(uint8_t irq_num) {
    switch (irq_num) {
        case 32:
            handleTimerInterrupt();
            break;
        case 33:
            handleKeyboardInterrupt();
            break;
        default:
            break;
    }
}

/*
Logo:

Welcome to
                ___________________
               /  ____      ______ \
  _  __ ______/ // __ \   // ____ \ \ 
         __    // /  \ \ // /   \\_\ \
        || |  // /    \ \\\ \_____   |
   ____ || |_ || |     | |\\_____ \  |
  //  _\||   \\\ \    / /___     \ \ |
  || |_ || |\ \\\ \__/ /_\\ \____/ / /
  \\___/||_| \______________________/
 --------------------------------------
*/
static const char* welcomelogo = 
"Welcome to\n"
"                ___________________\n"
"               /  ____      ______ \\\n"
"  _  __ ______/ // __ \\   // ____ \\ \\ \n"
"         __    // /  \\ \\ // /   \\\\_\\ \\\n"
"        || |  // /    \\ \\\\\\ \\_____   |\n"
"   ____ || |_ || |     | |\\\\_____ \\  |\n"
"  //  _\\||   \\\\\\ \\    / /___     \\ \\ |\n"
"  || |_ || |\\ \\\\\\ \\__/ /_\\\\ \\____/ / /\n"
"  \\\\___/||_| \\______________________/\n"
" --------------------------------------\n";

static constexpr uint16_t SOUND_LEN = 0xFFFF;
static uint8_t soundData[SOUND_LEN] = {}; 

/* note 0 = A */
static constexpr int NOTE_A = 0;
static constexpr int NOTE_AS = 1;
static constexpr int NOTE_BF = 1;
static constexpr int NOTE_B = 2;
static constexpr int NOTE_CF = 2;
static constexpr int NOTE_BS = 3;
static constexpr int NOTE_C = 3;
static constexpr int NOTE_CS = 4;
static constexpr int NOTE_D = 5;
static constexpr int NOTE_DS = 6;
static constexpr int NOTE_E = 7;
static constexpr int NOTE_F = 8;
static constexpr int NOTE_FS = 9;
static constexpr int NOTE_G = 10;
static constexpr int NOTE_GS = 11;
float getHzNote(int note, int octave) {
    float semitones = (float)note + (octave - 4) * 12;
    float hz = pow(2, semitones / 12) * 440;
    return hz;
}
float getHzA4Centered(float semitoneOffset) {
    float hz = pow(2, semitoneOffset / 12) * 440;
    return hz;
}

void playSound() {
    cli();

    float volume = 1.0f;
    float hz = 0.0f;
    for (int i = 0; i < SOUND_LEN; i++) {
        int div = SOUND_LEN / 256;
        // static constexpr int QUARTER = 16;
        if (i < div) {
            volume = 1.0f;
            hz = getHzNote(NOTE_D, 3);
        }
        else if (i < div * 2) {
            hz = getHzNote(NOTE_B, 4);
        }
        else if (i < div * 8) {
            hz = getHzNote(NOTE_G, 4);
        }
        else if (i < div * 16) {
            volume = 0.0f;
        }
        else if (i < div * 40) {
            volume = 1.0f;
            hz = getHzNote(NOTE_G, 4);
        }
        else if (i < div * 44) {
            hz = getHzNote(NOTE_FS, 4);
        }
        else if (i < div * 48) {
            hz = getHzNote(NOTE_G, 4);
        }
        else if (i < div * 52) {
            hz = getHzNote(NOTE_FS, 4);
        }
        else if (i < div * 56) {
            hz = getHzNote(NOTE_G, 4);
        }
        else if (i < div * 60) {
            hz = getHzNote(NOTE_FS, 4);
        }
        else if (i < div * 64) {
            hz = getHzNote(NOTE_G, 4);
        }
        else if (i < div * 66) {
            hz = getHzNote(NOTE_D, 3);
        }
        else if (i < div * 72) {
            hz = getHzNote(NOTE_D, 4);
        }
        else if (i < div * 80) {
            volume = 0.0f;
        }
        else if (i < div * 104) {
            volume = 1.0f;
            hz = getHzNote(NOTE_D, 4);
        }
        else if (i < div * 108) {
            hz = getHzNote(NOTE_CS, 4);
        }
        else if (i < div * 112) {
            hz = getHzNote(NOTE_D, 4);
        }
        else if (i < div * 116) {
            hz = getHzNote(NOTE_CS, 4);
        }
        else if (i < div * 120) {
            hz = getHzNote(NOTE_D, 4);
        }
        else if (i < div * 124) {
            hz = getHzNote(NOTE_CS, 4);
        }
        else if (i < div * 128) {
            hz = getHzNote(NOTE_D, 4);
        }
        else if (i < div * 130) {
            hz = getHzNote(NOTE_G, 2);
        }
        else if (i < div * 132) {
            hz = getHzNote(NOTE_D, 4);
        }
        else if (i < div * 138) {
            hz = getHzNote(NOTE_B, 4);
        }
        else if (i < div * 146) {
            volume = 0.0f;
        }
        else if (i < div * 170) {
            volume = 1.0f;
            hz = getHzNote(NOTE_B, 4);
        }
        else if (i < div * 174) {
            hz = getHzNote(NOTE_C, 4);
        }
        else if (i < div * 175) {
            hz = getHzNote(NOTE_D, 4);
        }
        else if (i < div * 176) {
            hz = getHzNote(NOTE_C, 4);
        }
        else if (i < div * 177) {
            hz = getHzNote(NOTE_D, 4);
        }
        else if (i < div * 178) {
            hz = getHzNote(NOTE_C, 4);
        }
        else if (i < div * 182) {
            hz = getHzNote(NOTE_D, 4);
        }
        else if (i < div * 186) {
            volume = 0.0f;
        }
        else if (i < div * 190) {
            volume = 1.0f;
            hz = getHzNote(NOTE_E, 4);
        }
        else if (i < div * 194) {
            volume = 0.0f;
        }
        else if (i < div * 202) {
            volume = 1.0f;
            hz = getHzNote(NOTE_G, 3);
        }
        else if (i < div * 210) {
            hz = getHzNote(NOTE_FS, 3);
        } 
        else if (i < div * 228) {
            hz = getHzNote(NOTE_FS, 3);
        } 
        else {
            volume = 0.0f;
        }
        float x = (float)i / 10989 * TAU * hz; 
        soundData[i] = volume * sin(x) + 1 * 256;
    }
    static constexpr uint16_t DSP_RESET_PORT = 0x226;
    static constexpr uint16_t DSP_WRITE_PORT = 0x22C;
    static constexpr uint8_t DSP_WRITE_CMD_SPEAKER_ON = 0xD1;
    static constexpr uint8_t DSP_WRITE_CMD_SET_TIME_CONST = 0x40;
    static constexpr uint16_t ISA_DMA_SINGLE_CHANNEL_MASK_REG = 0x0A;
    static constexpr uint16_t ISA_DMA_MODE_REG = 0x0B;
    static constexpr uint16_t ISA_DMA_FLIP_FLOP_RESET_REG = 0x0C;
    static constexpr uint16_t ISA_DMA_CHAN1_PAGE_ADDR_REG = 0x83;
    static constexpr uint16_t ISA_DMA_START_ADDR_CHAN1o5_REG = 0x02;
    static constexpr uint16_t ISA_DMA_COUNT_CHAN1o5_REG = 0x03;
    /* Reset DSP */
    outb(DSP_RESET_PORT, 1);
    // Wait 3 microseconds ish
    outb(DSP_RESET_PORT, 0);

    /* Turn speaker on */
    outb(DSP_WRITE_PORT, DSP_WRITE_CMD_SPEAKER_ON);

    static constexpr uint8_t CHANNEL_NUMBER = 1;
    static constexpr uint8_t CHANNEL_MASK_BIT = 0x04;

    outb(ISA_DMA_SINGLE_CHANNEL_MASK_REG, CHANNEL_NUMBER | CHANNEL_MASK_BIT);
    outb(ISA_DMA_FLIP_FLOP_RESET_REG, 0xFF);  // I believe value is arbitrary
    static constexpr uint8_t SINGLE_MODE = 0x48;
    outb(ISA_DMA_MODE_REG, SINGLE_MODE + CHANNEL_NUMBER);
    outb(ISA_DMA_CHAN1_PAGE_ADDR_REG, ((uint32_t)soundData >> 16) & 0xFF);
    outb(ISA_DMA_START_ADDR_CHAN1o5_REG, ((uint32_t)soundData >> 0) & 0xFF);
    outb(ISA_DMA_START_ADDR_CHAN1o5_REG, ((uint32_t)soundData >> 8) & 0xFF);
    outb(ISA_DMA_COUNT_CHAN1o5_REG, (SOUND_LEN - 1) & 0xFF);
    outb(ISA_DMA_COUNT_CHAN1o5_REG, ((SOUND_LEN - 1) >> 8) & 0xFF);
    outb(ISA_DMA_SINGLE_CHANNEL_MASK_REG, CHANNEL_NUMBER);
    
    outb(DSP_WRITE_PORT, DSP_WRITE_CMD_SET_TIME_CONST);
    outb(DSP_WRITE_PORT, 165);
    outb(DSP_WRITE_PORT, 0xC0);  // 8 bit
    outb(DSP_WRITE_PORT, 0x00);  // mono
    outb(DSP_WRITE_PORT, (SOUND_LEN - 1) & 0xFF);
    outb(DSP_WRITE_PORT, ((SOUND_LEN - 1) >> 8) & 0xFF);
    sti();
}

extern uint32_t _linker_end;

void output_multiboot_info() {
    char buf[100];
    const multiboot_info* info = get_multiboot_info();

    void* mem_start = (void*)&_linker_end;
    void* mem_end = 0;

    multiboot_mmap_entry *mmap;
    for (mmap = (multiboot_mmap_entry *) info->mmap_addr;
        (uint32_t) mmap < info->mmap_addr + info->mmap_length;
        mmap = (multiboot_mmap_entry *) ((uint32_t) mmap + mmap->size + sizeof (mmap->size)))
        {   
            if (mmap->type == 0x1 && mmap->addr_low == 0x100000) {
                mem_end = (void*) (mmap->len_low + 0x100000);
            }
        }
    terminal::writestring("== Multiboot Info ==\n");

    terminal::writestring("Memory: ");
    terminal::writestring(isFlagSet(MULTIBOOT_INFO_MEMORY) ? "Enabled" : "Disabled");
    terminal::writestring("\n");
    terminal::writestring("Boot Device: ");
    terminal::writestring(isFlagSet(MULTIBOOT_INFO_BOOTDEV) ? "Enabled" : "Disabled");
    terminal::writestring("\n");
    terminal::writestring("Command Line: ");
    terminal::writestring(isFlagSet(MULTIBOOT_INFO_CMDLINE) ? "Enabled" : "Disabled");
    terminal::writestring("\n");
    terminal::writestring("Modules: ");
    terminal::writestring(isFlagSet(MULTIBOOT_INFO_MODS) ? "Enabled" : "Disabled");
    terminal::writestring("\n");
    terminal::writestring("AOut: ");
    terminal::writestring(isFlagSet(MULTIBOOT_INFO_AOUT_SYMS) ? "Enabled" : "Disabled");
    terminal::writestring("\n");
    terminal::writestring("ELF Section Header: ");
    terminal::writestring(isFlagSet(MULTIBOOT_INFO_ELF_SHDR) ? "Enabled" : "Disabled");
    terminal::writestring("\n");
    terminal::writestring("Memory Map: ");
    terminal::writestring(isFlagSet(MULTIBOOT_INFO_MEM_MAP) ? "Enabled" : "Disabled");
    terminal::writestring("\n");
    terminal::writestring("Drives: ");
    terminal::writestring(isFlagSet(MULTIBOOT_INFO_DRIVE_INFO) ? "Enabled" : "Disabled");
    terminal::writestring("\n");
    terminal::writestring("Config Table: ");
    terminal::writestring(isFlagSet(MULTIBOOT_INFO_CONFIG_TABLE) ? "Enabled" : "Disabled");
    terminal::writestring("\n");
    terminal::writestring("Boot Loader: ");
    terminal::writestring(isFlagSet(MULTIBOOT_INFO_BOOT_LOADER_NAME) ? "Enabled" : "Disabled");
    terminal::writestring("\n");
    terminal::writestring("APM Table: ");
    terminal::writestring(isFlagSet(MULTIBOOT_INFO_APM_TABLE) ? "Enabled" : "Disabled");
    terminal::writestring("\n");
    terminal::writestring("VBE: ");
    terminal::writestring(isFlagSet(MULTIBOOT_INFO_VBE_INFO) ? "Enabled" : "Disabled");
    terminal::writestring("\n");
    terminal::writestring("Framebuffer: ");
    terminal::writestring(isFlagSet(MULTIBOOT_INFO_FRAMEBUFFER_INFO) ? "Enabled" : "Disabled");
    terminal::writestring("\n");
    terminal::writestring("\n");

    if (isFlagSet(MULTIBOOT_INFO_MEMORY)) {
        terminal::writestring("Lower Memory: 0x");
        terminal::writestring(terminal::itoa(info->mem_lower, buf, 16));
        terminal::writestring("\n");
        terminal::writestring("Upper Memory: 0x");
        terminal::writestring(terminal::itoa(info->mem_upper, buf, 16));
        terminal::writestring("\n");
        terminal::writestring("\n");
    }

    if (isFlagSet(MULTIBOOT_INFO_BOOTDEV)) {
        terminal::writestring("BIOS Drive Number: 0x");
        terminal::writestring(terminal::itoa((info->boot_device >> 24) & 0xFF, buf, 16));
        terminal::writestring("\n");
        terminal::writestring("Partition Level 1: 0x");
        terminal::writestring(terminal::itoa((info->boot_device >> 16) & 0xFF, buf, 16));
        terminal::writestring("\n");
        terminal::writestring("Partition Level 2: 0x");
        terminal::writestring(terminal::itoa((info->boot_device >> 8) & 0xFF, buf, 16));
        terminal::writestring("\n");
        terminal::writestring("Partition Level 3: 0x");
        terminal::writestring(terminal::itoa((info->boot_device >> 0) & 0xFF, buf, 16));
        terminal::writestring("\n");
        terminal::writestring("\n");
    }

    if (isFlagSet(MULTIBOOT_INFO_CMDLINE)) {
        terminal::writestring("Command Line: ");
        terminal::writestring((const char*) info->cmdline);
        terminal::writestring("\n");
        terminal::writestring("\n");
    }

    if (isFlagSet(MULTIBOOT_INFO_MODS)) {
        terminal::writestring("== Modules ==");
        terminal::writestring("\n");
        terminal::writestring("Module count: ");
        terminal::writestring(terminal::itoa(info->mods_count, buf, 10));
        terminal::writestring("\n");
        terminal::writestring("\n");

        multiboot_mod_entry* mod = (multiboot_mod_entry*) info->mods_addr;
        for (uint32_t i = 0; i < info->mods_count; i++) {
            terminal::writestring("Module name: ");
            terminal::writestring((const char*)(mod->string));
            terminal::writestring("\n");
            terminal::writestring("Start addr: 0x");
            terminal::writestring(terminal::itoa(mod->mod_start, buf, 16));
            terminal::writestring("\n");
            terminal::writestring("End addr: 0x");
            terminal::writestring(terminal::itoa(mod->mod_end, buf, 16));
            terminal::writestring("\n");
            terminal::writestring("\n");
            mod++;
        }
    }

    if (isFlagSet(MULTIBOOT_INFO_MEM_MAP)) {
        terminal::writestring("== Memory Map ==");
        terminal::writestring("\n");
        multiboot_mmap_entry *mmap;
        for (mmap = (multiboot_mmap_entry *) info->mmap_addr;
            (uint32_t) mmap < info->mmap_addr + info->mmap_length;
            mmap = (multiboot_mmap_entry *) ((uint32_t) mmap + mmap->size + sizeof (mmap->size)))
            {   
                terminal::writestring("Memory Address: 0x");
                terminal::writestring(terminal::itoa((int)mmap->addr_low, buf, 16));
                terminal::writestring("\n");
                terminal::writestring("Memory Region Length: 0x");
                terminal::writestring(terminal::itoa((int)mmap->len_low, buf, 16));
                terminal::writestring("\n");
                terminal::writestring("Memory Region Type: ");
                const char* typeStr;
                switch (mmap->type) {
                    case multiboot_mmap_entry::TYPE_AVAILABLE:
                        typeStr = "Available";
                        break;
                    case multiboot_mmap_entry::TYPE_RESERVED:
                        typeStr = "Reserved";
                        break;
                    case multiboot_mmap_entry::TYPE_ACPI_RECLAIMABLE:
                        typeStr = "ACPI Reclaimable";
                        break;
                    case multiboot_mmap_entry::TYPE_NVS:
                        typeStr = "NVS";
                        break;
                    case multiboot_mmap_entry::TYPE_BADRAM:
                        typeStr = "Bad Ram";
                        break;
                    default:
                        typeStr = "Reserved";
                        break;
                }
                terminal::writestring(typeStr);
                terminal::writestring("\n");
                terminal::writestring("\n");
            }
    }

    if (isFlagSet(MULTIBOOT_INFO_BOOT_LOADER_NAME)) {
        terminal::writestring("Booted by: ");
        terminal::writestring((const char*) (info->boot_loader_name));
        terminal::writestring("\n");
        terminal::writestring("\n");
    }

    terminal::writestring("Memory start: 0x");
    terminal::writestring(terminal::itoa((int)mem_start, buf, 16));
    terminal::writestring("\n");

    terminal::writestring("Memory end: 0x");
    terminal::writestring(terminal::itoa((int)mem_end, buf, 16));
    terminal::writestring("\n");
}

/* x86 registers that are callee-saved */
struct context_t {
    uint32_t ebx;  // Base
    uint32_t esi;  // Source
    uint32_t edi;  // Destination
    uint32_t ebp;  // Stack Base Pointer
    uint32_t eip;  // Instruction Pointer
};

extern "C"
void context_switch(context_t**, context_t*);
bool isproc1 = true;
context_t* ctx1;
context_t* ctx2;
void ctx_switch()
{
    if (isproc1)
    {
        isproc1 = false;
        context_switch(&ctx1, ctx2);
    }
    else 
    {
        isproc1 = true;
        context_switch(&ctx2, ctx1);
    }
}
uint8_t proc[2][4096];
uint32_t global = 9;
void proc1()
{
    while (global != 1)
    {
        if (global % 2 == 0)
        {
            terminal::writestring("Even Proc: ");
            terminal::writestring(terminal::itoa(global, terminal::volatile_itoa_buf, 10));
            global /= 2;
            terminal::writestring(" -> ");
            terminal::writestring(terminal::itoa(global, terminal::volatile_itoa_buf, 10));
            terminal::writestring("\n");
        }
        else
        {
            ctx_switch();
        }
    }
    terminal::writestring("We done :)");
    for (;;) {}
}

void proc2()
{
    while (global != 1)
    {
        if (global % 2 != 0)
        {
            terminal::writestring("Odd Proc: ");
            terminal::writestring(terminal::itoa(global, terminal::volatile_itoa_buf, 10));
            global = 3 * global + 1;
            terminal::writestring(" -> ");
            terminal::writestring(terminal::itoa(global, terminal::volatile_itoa_buf, 10));
            terminal::writestring("\n");
        }
        else
        {
            ctx_switch();
        }
    }
    for (;;) {}
}

extern "C"
void kernel_main(void) 
{
    /* Initialize */
	setup_gdt32();
    load_idt();
    terminal::initialize();
    terminal::writestring(welcomelogo);
    init_memory();
    uint8_t* sp1 = proc[0] + 4096;
    uint8_t* sp2 = proc[1] + 4096;

    sp1 -= 4;
    sp2 -= 4;

    sp1 -= sizeof(context_t);
    sp2 -= sizeof(context_t);

    ctx1 = (context_t*) sp1;
    ctx2 = (context_t*) sp2;

    ctx1->ebp = 0;
    ctx1->ebx = 0;
    ctx1->edi = 0;
    ctx1->esi = 0;
    ctx1->eip = (uint32_t) proc1;

    ctx2->ebp = 0;
    ctx2->ebx = 0;
    ctx2->edi = 0;
    ctx2->esi = 0;
    ctx2->eip = (uint32_t) proc2;

    proc1();
    for (;;){}

    const multiboot_info* info = get_multiboot_info();
    if (strncmp((const char*)(info->cmdline), "kernel terminal", 16) == 0) {
        terminal::initialize();
        output_multiboot_info();
        terminal::writestring(welcomelogo);
    }
    else if (strncmp((const char*)(info->cmdline), "kernel video", 13) == 0) {
        screen::init();
        world::init(screen::width(), screen::height());
        world::run();
    }


    for(;;) {}  // hang for now
}