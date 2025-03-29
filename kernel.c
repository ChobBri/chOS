#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

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

/* base between 2 and 36 */
char* itoa( int value, char* str, int base )
{
    char* rc;
    char* ptr;
    char* low;
    // Check for supported base.
    if ( base < 2 || base > 36 )
    {
        *str = '\0';
        return str;
    }
    ptr = str;
    rc = str;
    // Set '-' for negative decimals.
    if ( value < 0 && base == 10 )
    {
        *ptr++ = '-';
    }
    // Remember where the numbers start.
    low = ptr;
    // The actual conversion.
    do
    {
        // Modulo is negative for negative value. This trick makes abs() unnecessary.
        *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz"[35 + value % base];
        value /= base;
    } while ( value );
    // Terminating the string.
    *ptr-- = '\0';
    // Invert the numbers.
    while ( low < ptr )
    {
        char tmp = *low;
        *low++ = *ptr;
        *ptr-- = tmp;
    }
    return rc;
}

/* Hardware text mode color constants. */
typedef enum {
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
} vga_color;

static inline uint8_t vga_entry_color(vga_color fg, vga_color bg) 
{
	return fg | bg << 4;
}

static inline uint16_t vga_entry(char c, uint8_t color) 
{
	return (uint16_t) c | (uint16_t) color << 8;
}

size_t strlen(const char* str) 
{
	size_t len = 0;
	while (str[len])
		len++;
	return len;
}

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;
static const uint32_t VGA_MEMORY_BASE = 0xB8000;

typedef struct {
    size_t terminal_row;
    size_t terminal_column;
    uint8_t terminal_color;
    uint16_t* terminal_buffer;
} terminal_state;

static terminal_state state;

void terminal_updatecursor(size_t col, size_t row)
{
    uint16_t pos = row * VGA_WIDTH + col;

    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t) (pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
}

void terminal_clear_line(int row) {
    for (size_t col = 0; col < VGA_WIDTH; col++) {
        const int index = row * VGA_WIDTH + col;
        state.terminal_buffer[index] = vga_entry(' ', state.terminal_color);
    }
}

void terminal_initialize(void) 
{
    /* init fields */
	state.terminal_row = 0;
	state.terminal_column = 0;
	state.terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	state.terminal_buffer = (uint16_t*) VGA_MEMORY_BASE;

    /* clear screen */
	for (size_t row = 0; row < VGA_HEIGHT; row++) {
        terminal_clear_line(row);
	}

    terminal_updatecursor(state.terminal_column, state.terminal_row);
}

void terminal_setcolor(vga_color fg, vga_color bg) 
{
	state.terminal_color = vga_entry_color(fg, bg);
}

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) 
{
	const size_t index = y * VGA_WIDTH + x;
	state.terminal_buffer[index] = vga_entry(c, color);
}


/* positive is scroll text up */
void terminal_scroll(int scrollAmount) {
    if (scrollAmount > 0) {
        for (size_t row = scrollAmount; row < VGA_HEIGHT; row++) {
            for (size_t col = 0; col < VGA_WIDTH; col++) {
                const size_t old_index = row * VGA_WIDTH + col;
                const size_t new_index = (row - scrollAmount) * VGA_WIDTH + col;
                state.terminal_buffer[new_index] = state.terminal_buffer[old_index];
            }
            terminal_clear_line(row);
        }
    }
    else if (scrollAmount < 0) {
        for (int row = -scrollAmount; row >= 0; row--) {
            for (size_t col = 0; col < VGA_WIDTH; col++) {
                const size_t old_index = row * VGA_WIDTH + col;
                const size_t new_index = (row - scrollAmount) * VGA_WIDTH + col;
                state.terminal_buffer[new_index] = state.terminal_buffer[old_index];
            }
            terminal_clear_line(row);
        }
    }
}

void terminal_putchar(char c) 
{
    if (c == '\n') {
        state.terminal_column = 0;
        if (++state.terminal_row == VGA_HEIGHT) {
            state.terminal_row = VGA_HEIGHT - 1;
            terminal_scroll(1);
        }
    }
    else if (c == '\b') {
        if (state.terminal_column == 0) {
            if (state.terminal_row > 0) {
                state.terminal_row--;
                state.terminal_column = VGA_WIDTH - 1;
            }
        }
        else {
            state.terminal_column--;
        }
        terminal_putentryat(' ', state.terminal_color, state.terminal_column, state.terminal_row);
    }
    else {
        terminal_putentryat(c, state.terminal_color, state.terminal_column, state.terminal_row);
        if (++state.terminal_column == VGA_WIDTH) {
            state.terminal_column = 0;
            if (++state.terminal_row == VGA_HEIGHT) {
                state.terminal_row = VGA_HEIGHT - 1;
                terminal_scroll(1);
            }
        }
    }
    terminal_updatecursor(state.terminal_column, state.terminal_row);
}

void terminal_write(const char* data, size_t size) 
{
	for (size_t i = 0; i < size; i++) {
		terminal_putchar(data[i]);
    }
}

void terminal_writestring(const char* data) 
{
	terminal_write(data, strlen(data));
}


#define IDT_SIZE 256

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

char scancodeToChar(uint8_t scancode) {
    static char scancode_to_char[256] = {
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

    return scancode_to_char[scancode];
}

void interrupt_handler(uint8_t irq_num) {
    if (irq_num == 33) {
        uint8_t scancode = inb(0x60);
        char c = scancodeToChar(scancode);
        if (c != '\0') {
            terminal_putchar(c);
        }
    }
}

void kernel_main(void) 
{
    /* Initialize */
	terminal_initialize();
    load_idt();

    for(;;) {}  // hang for now
}