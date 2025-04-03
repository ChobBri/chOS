#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "keycode.h"
#include "terminal.h"
#include "gdt.h"

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

void set_timer_phase(int hz) {
    int divisor = 1193182 / hz;
    outb(0x43, 0x36);       // Set mode: square wave generator
    outb(0x40, divisor & 0xFF);
    outb(0x40, divisor >> 8);
}

extern void irq_stub_32();
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

void handleKeyboardInterrupt(){
    static bool shift = false;
    static bool capslock = false;
    static bool e0 = false;
    uint8_t scancode = inb(0x60);

    if (scancode == 0xe0) {
        e0 = true;
        return;
    }
    
    
    keycode kc;
    bool pressed;

    if (!e0) {
        kc = scancodeToKeycode(scancode);
        pressed = scancodeToPressed(scancode);
    }
    else {
        kc = scancodeToKeycode2(scancode);
        pressed = scancodeToPressed2(scancode);
    }
    e0 = false;
    
    if (pressed && (kc == LeftShift || kc == RightShift)) {
        shift = true;
    } else if (!pressed && (kc == LeftShift || kc == RightShift)) {
        shift = false;
    }

    if (pressed && kc == CapsLock) {
        capslock = !capslock;
    }

    char c = keycodeToChar(kc, shift ^ capslock);
    if (c != '\0' && pressed) {
        terminal_putchar(c);
    }
}

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

void kernel_main(void) 
{
    /* Initialize */
	setup_gdt32();
    terminal_initialize();
    terminal_writestring("\n");
    terminal_writestring(welcomelogo);
    terminal_writestring("\n> ");
    load_idt();
    for(;;) {}  // hang for now
}