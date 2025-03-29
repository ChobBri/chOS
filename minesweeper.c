#include "minesweeper.h"
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
/* Hardware text mode color constants. */
typedef enum {
	DISPLAY_COLOR_BLACK = 0,
	DISPLAY_COLOR_BLUE = 1,
	DISPLAY_COLOR_GREEN = 2,
	DISPLAY_COLOR_CYAN = 3,
	DISPLAY_COLOR_RED = 4,
	DISPLAY_COLOR_MAGENTA = 5,
	DISPLAY_COLOR_BROWN = 6,
	DISPLAY_COLOR_LIGHT_GREY = 7,
	DISPLAY_COLOR_DARK_GREY = 8,
	DISPLAY_COLOR_LIGHT_BLUE = 9,
	DISPLAY_COLOR_LIGHT_GREEN = 10,
	DISPLAY_COLOR_LIGHT_CYAN = 11,
	DISPLAY_COLOR_LIGHT_RED = 12,
	DISPLAY_COLOR_LIGHT_MAGENTA = 13,
	DISPLAY_COLOR_LIGHT_BROWN = 14,
	DISPLAY_COLOR_WHITE = 15,
} display_color;

#define DISPLAY_WIDTH 80
#define DISPLAY_HEIGHT 25
static const uint32_t DISPLAY_MEMORY_BASE = 0xB8000;
static uint16_t* DISPLAY_BUFFER = (uint16_t*) DISPLAY_MEMORY_BASE;
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

static inline uint8_t display_entry_color(display_color fg, display_color bg) 
{
	return fg | bg << 4;
}

static inline uint16_t display_entry(char c, uint8_t color) 
{
	return (uint16_t) c | (uint16_t) color << 8;
}

void display_putentryat(char c, uint8_t color, size_t x, size_t y) 
{
    const size_t index = y * DISPLAY_WIDTH + x;
    DISPLAY_BUFFER[index] = display_entry(c, color);
}

static void set_timer_phase(int hz) {
    int divisor = 1193182 / hz;
    outb(0x43, 0x36);       // Set mode: square wave generator
    outb(0x40, divisor & 0xFF);
    outb(0x40, divisor >> 8);
}

static char scancodeToChar(uint8_t scancode, bool shift) {
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


void init_game();

void play_game() {
    init_game();
    for(;;){};
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

#define IDT_SIZE 256
__attribute__((aligned(0x10))) 
static idt_entry idt[IDT_SIZE];

// IDT Descriptor
static idt_descriptor idtr;
static void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags) {
    idt[num].offset_low  = base & 0xFFFF;        // Low 16 bits of ISR address
    idt[num].selector    = sel;                  // Kernel segment selector (0x08)
    idt[num].zero        = 0;                    // Reserved
    idt[num].type_attr   = flags;                // Type and attributes
    idt[num].offset_high = (base >> 16) & 0xFFFF; // High 16 bits of ISR address
}

extern void ms_irq_stub_32();
extern void ms_irq_stub_33();

typedef struct {
    bool revealed;
    bool bomb;
    int hint;
} Tile;

#define BOARD_WIDTH 30
#define BOARD_HEIGHT 16
Tile board[BOARD_WIDTH * BOARD_HEIGHT];

typedef struct {
    size_t col;
    size_t row;
    bool dead;
} GameState;
GameState gameState;

static void handleTimerInterrupt() {
    /* clear screen */
    if (!gameState.dead) {
        for (size_t row = 0; row < BOARD_HEIGHT; row++) {
            for (size_t col = 0; col < BOARD_WIDTH; col++) {
                bool cursorPosition = (gameState.col == col && gameState.row == row);
                size_t index = row * BOARD_WIDTH + col;
                Tile tile = board[index];
                if (!tile.revealed) {
                    uint8_t color = display_entry_color(DISPLAY_COLOR_LIGHT_GREY, cursorPosition ? DISPLAY_COLOR_LIGHT_CYAN : DISPLAY_COLOR_LIGHT_GREY);
                    display_putentryat(' ', color, col, row);
                }
                else {
                    if (tile.bomb) {
                        uint8_t color = display_entry_color(DISPLAY_COLOR_LIGHT_RED, cursorPosition ? DISPLAY_COLOR_LIGHT_BROWN : DISPLAY_COLOR_DARK_GREY );
                        display_putentryat('\13', color, col, row);
                    }
                    else {
                        char c = tile.hint + '0';
                        if (c == '0') {
                            c = ' ';
                        }
                        uint8_t color = display_entry_color(cursorPosition ? DISPLAY_COLOR_BLACK : DISPLAY_COLOR_WHITE, cursorPosition ? DISPLAY_COLOR_LIGHT_BROWN : DISPLAY_COLOR_DARK_GREY);
                        display_putentryat(c, color, col, row);
                    }
                }
            }
        }
    }
    else {
        for (size_t row = 0; row < BOARD_HEIGHT; row++) {
            for (size_t col = 0; col < BOARD_WIDTH; col++) {
                static int fg = 0;
                static int bg = 8;
                uint8_t color = display_entry_color(fg++, bg++);
                display_putentryat(' ', color, col, row);
            }
        }
    }
}

void revealTile(size_t col, size_t row) {
    size_t index = row * BOARD_WIDTH + col;
    if (board[index].revealed) {
        return;
    }

    board[index].revealed = true;
    if (board[index].bomb || board[index].hint > 0) {
        return;
    }
    if (row > 0) {
        if (col > 0) {
            revealTile(col - 1, row - 1);
        }
        revealTile(col, row - 1);
        if (col < BOARD_WIDTH - 1) {
            revealTile(col + 1, row - 1);
        }
    }
    if (col > 0) {
        revealTile(col - 1, row);
    }
    revealTile(col, row);
    if (col < BOARD_WIDTH - 1) {
        revealTile(col + 1, row);
    }
    if (row < BOARD_HEIGHT - 1) {
        if (col > 0) {
            revealTile(col - 1, row + 1);
        }
        revealTile(col, row + 1);
        if (col < BOARD_WIDTH - 1) {
            revealTile(col + 1, row + 1);
        }
    }
}

static void handleKeyboardInterrupt(){
    static bool shift = false;
    uint8_t scancode = inb(0x60);
    
    if (scancode == 0x2A) {
        shift = true;
    } else if (scancode == 0xAA) {
        shift = false;
    }
    char c = scancodeToChar(scancode, shift);
    if (c != '\0') {
    }
    if (c == 'i') {
        if (gameState.row > 0) {
            gameState.row--;
        }
    }
    else if (c == 'k') {
        if (gameState.row < BOARD_HEIGHT - 1) {
            gameState.row++;
        }
    }
    else if (c == 'j') {
        if (gameState.col > 0) {
            gameState.col--;
        }
    } else if (c == 'l') {
        if (gameState.col < BOARD_WIDTH - 1) {
            gameState.col++;
        }
    }

    if (c == ' ') {
        size_t index = gameState.row * BOARD_WIDTH + gameState.col;
        if (!board[index].revealed) {
            revealTile(gameState.col, gameState.row);
            if (board[index].bomb) {
                gameState.dead = true;
            }
        }
    }
}

void ms_interrupt_handler(uint8_t irq_num) {
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

void display_initialize();

extern void ms_irq_stub_32();
extern void ms_irq_stub_33();
void init_game() {
    asm volatile ("cli");
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
    display_initialize();

    idt_set_gate(32, (uint32_t) ms_irq_stub_32, 0x08, 0x8E);
    idt_set_gate(33, (uint32_t) ms_irq_stub_33, 0x08, 0x8E);
    
    asm volatile ("lidt %0" : : "m" (idtr));
    
	for (size_t row = 0; row < BOARD_HEIGHT; row++) {
        for (size_t col = 0; col < BOARD_WIDTH; col++) {
            size_t index = row * BOARD_WIDTH + col;
            board[index].revealed = false;
            board[index].bomb = false;
            board[index].hint = 0;
        }
	}

    board[5].bomb = true;
    board[10].bomb = true;
    board[12].bomb = true;
    board[22].bomb = true;
    board[23].bomb = true;
    board[27].bomb = true;
    board[28].bomb = true;

    board[37].bomb = true;
    board[44].bomb = true;
    board[46].bomb = true;
    board[52].bomb = true;

    board[60].bomb = true;
    board[67].bomb = true;
    board[72].bomb = true;
    board[86].bomb = true;
    board[89].bomb = true;

    board[100].bomb = true;
    board[102].bomb = true;
    board[103].bomb = true;
    board[112].bomb = true;
    board[113].bomb = true;
    board[114].bomb = true;
    board[115].bomb = true;
    board[118].bomb = true;

    board[131].bomb = true;
    board[137].bomb = true;
    board[143].bomb = true;
    board[145].bomb = true;
    board[146].bomb = true;
    
    board[155].bomb = true;
    board[157].bomb = true;
    board[164].bomb = true;
    board[166].bomb = true;
    board[169].bomb = true;
    board[173].bomb = true;
    
    board[185].bomb = true;
    board[187].bomb = true;
    board[191].bomb = true;
    board[192].bomb = true;
    board[193].bomb = true;
    board[196].bomb = true;
    board[197].bomb = true;
    board[198].bomb = true;
    board[206].bomb = true;
    
    board[210].bomb = true;
    board[211].bomb = true;
    board[214].bomb = true;
    board[216].bomb = true;
    board[218].bomb = true;
    board[238].bomb = true;
    
    board[240].bomb = true;
    board[245].bomb = true;
    board[246].bomb = true;
    board[253].bomb = true;
    board[259].bomb = true;
    board[269].bomb = true;
    
    board[270].bomb = true;
    board[274].bomb = true;
    board[279].bomb = true;
    board[296].bomb = true;
    board[299].bomb = true;
    
    board[300].bomb = true;
    board[302].bomb = true;
    board[304].bomb = true;
    board[306].bomb = true;
    board[310].bomb = true;
    board[311].bomb = true;
    board[328].bomb = true;
    
    board[334].bomb = true;
    board[336].bomb = true;
    board[339].bomb = true;
    board[348].bomb = true;
    board[353].bomb = true;
    board[355].bomb = true;
    
    board[362].bomb = true;
    board[364].bomb = true;
    board[367].bomb = true;
    board[381].bomb = true;
    board[384].bomb = true;
    
    board[400].bomb = true;
    board[407].bomb = true;
    board[409].bomb = true;
    
    board[420].bomb = true;
    board[421].bomb = true;
    board[422].bomb = true;
    board[435].bomb = true;
    board[436].bomb = true;
    board[437].bomb = true;
    board[438].bomb = true;
    board[447].bomb = true;
    
    board[452].bomb = true;
    board[456].bomb = true;
    board[459].bomb = true;
    board[461].bomb = true;
    board[462].bomb = true;
    board[468].bomb = true;
    board[470].bomb = true;
    board[471].bomb = true;
    board[477].bomb = true;

    for (int row = 0; row < BOARD_HEIGHT; row++) {
        for (int col = 0; col < BOARD_WIDTH; col++) {
            int index = row * BOARD_WIDTH + col;
            int sum = 0;

            for (int orow = -1; orow <= 1; orow++) {
                for (int ocol = -1; ocol <= 1; ocol++) {
                    if (orow == 0 && ocol == 0) {
                        continue;
                    }
                    int newrow = (row + orow);
                    int newcol = (col + ocol);
                    int nindex = newrow * BOARD_WIDTH + newcol;
                    if (newrow >= 0 && newrow < BOARD_HEIGHT && newcol >= 0 && newcol < BOARD_WIDTH && board[nindex].bomb) {
                        sum++;
                    }
                }
            }
            board[index].hint = sum;
        }
	}

    gameState.col = 0;
    gameState.row = 0;
    gameState.dead = false;

    asm volatile ("sti");
}




void disable_cursor()
{
	outb(0x3D4, 0x0A);
	outb(0x3D5, 0x20);
}



void display_initialize(void) 
{
    disable_cursor();
    /* clear screen */
	for (size_t row = 0; row < DISPLAY_HEIGHT; row++) {
        for (size_t col = 0; col < DISPLAY_WIDTH; col++) {
            uint8_t color = display_entry_color(DISPLAY_COLOR_WHITE, DISPLAY_COLOR_BLACK);
            display_putentryat(' ', color, col, row);
        }
	}
}
