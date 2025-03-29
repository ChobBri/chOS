#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "terminal.h"

#define IDT_SIZE 256
static inline void outb(uint16_t port, uint8_t val)
{
    __asm__ volatile ( "outb %b0, %w1" : : "a"(val), "Nd"(port) : "memory");
    /* There's an outb %al, $imm8 encoding, for compile-time constant port numbers that fit in 8b. (N constraint).
     * Wider immediate constants would be truncated at assemble-time (e.g. "i" constraint).
     * The  outb  %al, %dx  encoding is the only option for all other cases.
     * %1 expands to %dx because  port  is a uint16_t.  %w1 could be used if we had the port number a wider C type */
}

static inline uint8_t inb(uint16_t port)
{
    uint8_t ret;
    __asm__ volatile ( "inb %w1, %b0"
                   : "=a"(ret)
                   : "Nd"(port)
                   : "memory");
    return ret;
}

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

extern void irq_stub_33();
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

        "movb $0xFD, %al\n"  // Enable IRQ1 keyboard
        "outb %al, $0x21\n"
        "movb $0xFF, %al\n"
        "outb %al, $0xA1\n"
    );
    idtr.limit = sizeof(idt) - 1;
    idtr.base = (uint32_t) &idt;

    idt_set_gate(33, (uint32_t) irq_stub_33, 0x08, 0x8E);
    
    asm volatile ("lidt %0" : : "m" (idtr));
    asm volatile ("sti");
}

char scancodeToChar(uint8_t scancode, bool shift) {
    static const char charTable[256] = {
        [0x1E] = 'a', [0x30] = 'b', [0x2E] = 'c', [0x20] = 'd', [0x12] = 'e',
        [0x21] = 'f', [0x22] = 'g', [0x23] = 'h', [0x17] = 'i', [0x24] = 'j',
        [0x25] = 'k', [0x26] = 'l', [0x32] = 'm', [0x31] = 'n', [0x18] = 'o',
        [0x19] = 'p', [0x10] = 'q', [0x13] = 'r', [0x1f] = 's', [0x14] = 't',
        [0x16] = 'u', [0x2F] = 'v', [0x11] = 'w', [0x2D] = 'x', [0x15] = 'y',
        [0x2C] = 'z',
        [0x0B] = '0', [0x02] = '1', [0x03] = '2', [0x04] = '3', [0x05] = '4',
        [0x06] = '5', [0x07] = '6', [0x08] = '7', [0x09] = '8', [0x0A] = '9',
        [0x1C] = '\n', [0x0F] = '\t', [0x39] = ' ', [0x0E] = '\b',
        [0x34] = '.', [0x33] = ',', [0x35] = '/', [0x0C] = '-', [0x0D] = '=',
        [0x1A] = '[', [0x1B] = ']', [0x27] = ';', [0x28] = '\'', [0x29] = '`',
        [0x2B] = '\\',
    };
    static const char shiftCharTable[256] = {
        [0x1E] = 'A', [0x30] = 'B', [0x2E] = 'C', [0x20] = 'D', [0x12] = 'E',
        [0x21] = 'F', [0x22] = 'G', [0x23] = 'H', [0x17] = 'I', [0x24] = 'J',
        [0x25] = 'K', [0x26] = 'L', [0x32] = 'M', [0x31] = 'N', [0x18] = 'O',
        [0x19] = 'P', [0x10] = 'Q', [0x13] = 'R', [0x1f] = 'S', [0x14] = 'T',
        [0x16] = 'U', [0x2F] = 'V', [0x11] = 'W', [0x2D] = 'X', [0x15] = 'Y',
        [0x2C] = 'Z',
        [0x0B] = ')', [0x02] = '!', [0x03] = '@', [0x04] = '#', [0x05] = '$',
        [0x06] = '%', [0x07] = '^', [0x08] = '&', [0x09] = '*', [0x0A] = '(',
        [0x1C] = '\n', [0x0F] = '\t', [0x39] = ' ', [0x0E] = '\b',
        [0x34] = '>', [0x33] = '<', [0x35] = '?', [0x0C] = '_', [0x0D] = '+',
        [0x1A] = '{', [0x1B] = '}', [0x27] = ':', [0x28] = '"', [0x29] = '~',
        [0x2B] = '|',
    };

    if (shift) {
        return shiftCharTable[scancode];
    } else {
        return charTable[scancode];
    }
}

static bool shift = false;

void interrupt_handler(uint8_t irq_num) {
    if (irq_num == 33) {
        uint8_t scancode = inb(0x60);

        if (scancode == 0x2A) {
            shift = true;
        } else if (scancode == 0xAA) {
            shift = false;
        }
        char c = scancodeToChar(scancode, shift);
        if (c != '\0') {
            terminal_putchar(c);
        }
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

void kernel_main(void) 
{
    /* Initialize */
	terminal_initialize();
    load_idt();

    terminal_writestring("\n");
    terminal_writestring(welcomelogo);
    terminal_writestring("\n> ");
    for(;;) {}  // hang for now
}