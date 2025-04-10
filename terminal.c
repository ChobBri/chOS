#include <stdint.h>
#include <stddef.h>
#include "terminal.h"
#include "vga.h"
#include "string.h"

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
    size_t row_pos;
    size_t col_pos;
    uint8_t color;
    uint16_t* buffer;
    uint8_t line_len[VGA_HEIGHT];
} terminal_state;

static terminal_state state;

void terminal_init_mode3() {
    vga_write_atrb_reg(0x10, 0x0C);
    vga_write_atrb_reg(0x11, 0x00);
    vga_write_atrb_reg(0x12, 0x0F);
    vga_write_atrb_reg(0x13, 0x08);
    vga_write_atrb_reg(0x14, 0x00);

    vga_write_misc_output_reg(0x67);

    vga_write_seq_reg(0x01, 0x00);
    vga_write_seq_reg(0x03, 0x00);
    vga_write_seq_reg(0x04, 0x07);

    vga_write_gfx_reg(0x05, 0x10);
    vga_write_gfx_reg(0x06, 0x0E);

    vga_write_crt_ctrl_reg(0x00, 0x5F);
    vga_write_crt_ctrl_reg(0x01, 0x4F);
    vga_write_crt_ctrl_reg(0x02, 0x50);
    vga_write_crt_ctrl_reg(0x03, 0x82);
    vga_write_crt_ctrl_reg(0x04, 0x55);
    vga_write_crt_ctrl_reg(0x05, 0x81);
    vga_write_crt_ctrl_reg(0x06, 0xBF);
    vga_write_crt_ctrl_reg(0x07, 0x1F);
    vga_write_crt_ctrl_reg(0x08, 0x00);
    vga_write_crt_ctrl_reg(0x09, 0x4F);
    vga_write_crt_ctrl_reg(0x10, 0x9C);
    vga_write_crt_ctrl_reg(0x11, 0x8E);
    vga_write_crt_ctrl_reg(0x12, 0x8F);
    vga_write_crt_ctrl_reg(0x13, 0x28);
    vga_write_crt_ctrl_reg(0x14, 0x1F);
    vga_write_crt_ctrl_reg(0x15, 0x96);
    vga_write_crt_ctrl_reg(0x16, 0xB9);
    vga_write_crt_ctrl_reg(0x17, 0xA3);
}

void terminal_init_mode13() {
    vga_write_misc_output_reg(0x63);
    
    vga_write_seq_reg(0x00, 0x03);
    vga_write_seq_reg(0x01, 0x01);
    vga_write_seq_reg(0x02, 0x0F);
    vga_write_seq_reg(0x03, 0x00);
    vga_write_seq_reg(0x04, 0x0E);

    vga_write_atrb_reg(0x00, 0x00);
    vga_write_atrb_reg(0x01, 0x01);
    vga_write_atrb_reg(0x02, 0x02);
    vga_write_atrb_reg(0x03, 0x03);
    vga_write_atrb_reg(0x04, 0x04);
    vga_write_atrb_reg(0x05, 0x05);
    vga_write_atrb_reg(0x06, 0x06);
    vga_write_atrb_reg(0x07, 0x07);
    vga_write_atrb_reg(0x08, 0x08);
    vga_write_atrb_reg(0x09, 0x09);
    vga_write_atrb_reg(0x0A, 0x0A);
    vga_write_atrb_reg(0x0B, 0x0B);
    vga_write_atrb_reg(0x0C, 0x0C);
    vga_write_atrb_reg(0x0D, 0x0D);
    vga_write_atrb_reg(0x0E, 0x0E);
    vga_write_atrb_reg(0x0F, 0x0F);

    vga_write_atrb_reg(0x10, 0x41);
    vga_write_atrb_reg(0x11, 0x00);
    vga_write_atrb_reg(0x12, 0x0F);
    vga_write_atrb_reg(0x13, 0x00);
    vga_write_atrb_reg(0x14, 0x00);
    


    vga_write_gfx_reg(0x00, 0x00);
    vga_write_gfx_reg(0x01, 0x00);
    vga_write_gfx_reg(0x02, 0x00);
    vga_write_gfx_reg(0x03, 0x00);
    vga_write_gfx_reg(0x04, 0x00);
    vga_write_gfx_reg(0x05, 0x40);
    vga_write_gfx_reg(0x06, 0x05);
    vga_write_gfx_reg(0x07, 0x0F);
    vga_write_gfx_reg(0x08, 0xFF);

    vga_write_crt_ctrl_reg(0x03, vga_read_crt_ctrl_reg(0x03) | 0x80);
    vga_write_crt_ctrl_reg(0x11, vga_read_crt_ctrl_reg(0x11) & ~0x80);

    vga_write_crt_ctrl_reg(0x00, 0x5F);
    vga_write_crt_ctrl_reg(0x01, 0x4F);
    vga_write_crt_ctrl_reg(0x02, 0x50);
    vga_write_crt_ctrl_reg(0x03, 0x82);
    vga_write_crt_ctrl_reg(0x04, 0x54);
    vga_write_crt_ctrl_reg(0x05, 0x80);
    vga_write_crt_ctrl_reg(0x06, 0xBF);
    vga_write_crt_ctrl_reg(0x07, 0x1F);
    vga_write_crt_ctrl_reg(0x08, 0x00);
    vga_write_crt_ctrl_reg(0x09, 0x41);
    vga_write_crt_ctrl_reg(0x0A, 0x00);
    vga_write_crt_ctrl_reg(0x0B, 0x00);
    vga_write_crt_ctrl_reg(0x0C, 0x00);
    vga_write_crt_ctrl_reg(0x0D, 0x00);
    vga_write_crt_ctrl_reg(0x0E, 0x00);
    vga_write_crt_ctrl_reg(0x0F, 0x00);
    vga_write_crt_ctrl_reg(0x10, 0x9C);
    vga_write_crt_ctrl_reg(0x11, 0x8E);
    vga_write_crt_ctrl_reg(0x12, 0x8F);
    vga_write_crt_ctrl_reg(0x13, 0x28);
    vga_write_crt_ctrl_reg(0x14, 0x40);
    vga_write_crt_ctrl_reg(0x15, 0x96);
    vga_write_crt_ctrl_reg(0x16, 0xB9);
    vga_write_crt_ctrl_reg(0x17, 0xA3);
    vga_write_crt_ctrl_reg(0x18, 0xFF);
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
        state.buffer[index] = vga_entry(' ', state.color);
    }
    state.line_len[row] = 0;
}

void terminal_initialize(void) 
{
    /* init fields */
	state.row_pos = 0;
	state.col_pos = 0;
	state.color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	state.buffer = (uint16_t*) VGA_MEMORY_BASE;
	for (size_t row = 0; row < VGA_HEIGHT; row++) {
        state.line_len[row] = 0;
	}

    /* clear screen */
	for (size_t row = 0; row < VGA_HEIGHT; row++) {
        terminal_clear_line(row);
	}

    terminal_updatecursor(state.col_pos, state.row_pos);
}

void terminal_setcolor(vga_color fg, vga_color bg) 
{
	state.color = vga_entry_color(fg, bg);
}

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) 
{
	const size_t index = y * VGA_WIDTH + x;
	state.buffer[index] = vga_entry(c, color);
}


/* positive is scroll text up */
void terminal_scroll(int scrollAmount) {
    if (scrollAmount > 0) {
        for (size_t row = scrollAmount; row < VGA_HEIGHT; row++) {
            state.line_len[row - scrollAmount] = state.line_len[row];
            for (size_t col = 0; col < VGA_WIDTH; col++) {
                const size_t old_index = row * VGA_WIDTH + col;
                const size_t new_index = (row - scrollAmount) * VGA_WIDTH + col;
                state.buffer[new_index] = state.buffer[old_index];
            }
            terminal_clear_line(row);
        }
    }
    else if (scrollAmount < 0) {
        for (int row = -scrollAmount; row >= 0; row--) {
            state.line_len[row - scrollAmount] = state.line_len[row];
            for (size_t col = 0; col < VGA_WIDTH; col++) {
                const size_t old_index = row * VGA_WIDTH + col;
                const size_t new_index = (row - scrollAmount) * VGA_WIDTH + col;
                state.buffer[new_index] = state.buffer[old_index];
            }
            terminal_clear_line(row);
        }
    }
}

void terminal_putchar(char c) 
{
    if (c == '\n') {
        state.line_len[state.row_pos] = state.col_pos;
        state.col_pos = 0;
        if (++state.row_pos == VGA_HEIGHT) {
            state.row_pos = VGA_HEIGHT - 1;
            terminal_scroll(1);
        }
    }
    else if (c == '\b') {
        if (state.col_pos == 0) {
            if (state.row_pos > 0) {
                state.row_pos--;
                state.col_pos = state.line_len[state.row_pos];
                if (state.col_pos >= VGA_WIDTH) {
                    state.col_pos = VGA_WIDTH - 1;
                }
            }
        }
        else {
            state.col_pos--;
            state.line_len[state.row_pos] = state.col_pos;
        }
        terminal_putentryat(' ', state.color, state.col_pos, state.row_pos);
    }
    else {
        terminal_putentryat(c, state.color, state.col_pos, state.row_pos);
        state.line_len[state.row_pos] = state.col_pos + 1;
        if (++state.col_pos == VGA_WIDTH) {
            state.col_pos = 0;
            if (++state.row_pos == VGA_HEIGHT) {
                state.row_pos = VGA_HEIGHT - 1;
                terminal_scroll(1);
            }
        }
    }
    terminal_updatecursor(state.col_pos, state.row_pos);
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
