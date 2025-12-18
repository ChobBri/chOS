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

static constexpr uint16_t SOUND_LEN = 0x4000;
static uint8_t soundData[SOUND_LEN] = {}; 

void playSound() {
    cli();
    for (int i = 0; i < SOUND_LEN; i++) {
        int factor = i / (SOUND_LEN / 8) + 2;
        soundData[i] = (i * factor) % 256;
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
    for (int i = 0; i < 10000; i++) {
        terminal::writestring("");
    }
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

extern "C"
void kernel_main(void) 
{
    /* Initialize */
	setup_gdt32();

    init_memory();
    
    load_idt();

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
        // draw_logo();
    }


    for(;;) {}  // hang for now
}