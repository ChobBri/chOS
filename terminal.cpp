#include <stdint.h>
#include <stddef.h>
#include "terminal.h"
#include "vga.h"
#include "string.h"
#include "keyboard.h"
#include "math.h"
#include "x86.h"
#include "char.h"

namespace terminal {
using namespace vga;

typedef struct {
    int row_pos;
    int col_pos;
    uint8_t color;
    uint16_t* buffer;
    uint8_t line_len[VGA_HEIGHT];
} terminal_state;

static terminal_state state;

using namespace vga;

void deleteChar();

void init_mode3() {
    vga::write_atrb_reg(0x10, 0x0C);
    vga::write_atrb_reg(0x11, 0x00);
    vga::write_atrb_reg(0x12, 0x0F);
    vga::write_atrb_reg(0x13, 0x08);
    vga::write_atrb_reg(0x14, 0x00);

    vga::write_misc_output_reg(0x67);

    vga::write_seq_reg(0x01, 0x00);
    vga::write_seq_reg(0x03, 0x00);
    vga::write_seq_reg(0x04, 0x07);

    vga::write_gfx_reg(0x05, 0x10);
    vga::write_gfx_reg(0x06, 0x0E);

    vga::write_crt_ctrl_reg(0x00, 0x5F);
    vga::write_crt_ctrl_reg(0x01, 0x4F);
    vga::write_crt_ctrl_reg(0x02, 0x50);
    vga::write_crt_ctrl_reg(0x03, 0x82);
    vga::write_crt_ctrl_reg(0x04, 0x55);
    vga::write_crt_ctrl_reg(0x05, 0x81);
    vga::write_crt_ctrl_reg(0x06, 0xBF);
    vga::write_crt_ctrl_reg(0x07, 0x1F);
    vga::write_crt_ctrl_reg(0x08, 0x00);
    vga::write_crt_ctrl_reg(0x09, 0x4F);
    vga::write_crt_ctrl_reg(0x10, 0x9C);
    vga::write_crt_ctrl_reg(0x11, 0x8E);
    vga::write_crt_ctrl_reg(0x12, 0x8F);
    vga::write_crt_ctrl_reg(0x13, 0x28);
    vga::write_crt_ctrl_reg(0x14, 0x1F);
    vga::write_crt_ctrl_reg(0x15, 0x96);
    vga::write_crt_ctrl_reg(0x16, 0xB9);
    vga::write_crt_ctrl_reg(0x17, 0xA3);
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

void updatecursor(int col, int row)
{
    uint16_t pos = row * VGA_WIDTH + col;

    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t) (pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));

    state.col_pos = col;
    state.row_pos = row;
}

/* dir is clamped between -1 and 1 */
void shiftCursorHorizontally(int dir) {
    dir = clamp(dir, -1, 1);
    int newCol = state.col_pos + dir;

    if (newCol < 0 || newCol > state.line_len[state.row_pos]) {
        return;
    }
    updatecursor(newCol, state.row_pos);
}
/* col/rowDir is clamped between -1 and 1 */
void shiftCursorVertically(int dir) {
    dir = clamp(dir, -1, 1);
    int newRow = state.row_pos - dir;

    if (newRow < 0 || newRow >= VGA_HEIGHT) {
        return;
    }

    int newCol = clamp(state.col_pos, 0, state.line_len[newRow]);
    updatecursor(newCol, newRow);
}

void clear_line(int row) {
    for (int col = 0; col < VGA_WIDTH; col++) {
        const int index = row * VGA_WIDTH + col;
        state.buffer[index] = vga::entry(' ', state.color);
    }
    state.line_len[row] = 0;
}

void handleKeyboardInput(keycode kc, bool pressed) {
    if (!pressed) {
        return;
    }

    if (kc == None) {
        return;
    }

    bool shift = keyboard::isKeyPressed(LeftShift) || keyboard::isKeyPressed(RightShift);
    bool capslock = keyboard::isCapsLockOn();
    char dummyC = keycodeToChar(kc, false);
    if (!isAlpha(dummyC)) {
        capslock = false;
    }
    char c = keycodeToChar(kc, shift ^ capslock);
    if (pressed) {
        if (kc == LeftArrow) {
            shiftCursorHorizontally(-1);
        }
        else if (kc == RightArrow) {
            shiftCursorHorizontally(1);
        }
        else if (kc == UpArrow) {
            shiftCursorVertically(1);
        }
        else if (kc == DownArrow) {
            shiftCursorVertically(-1);
        }
        else if (kc == Backspace) {
            deleteChar();
        }
        else {
            if (c != '\0') {
                terminal::putchar(c);
            }
        }
    }
}

void initialize(void) 
{
    /* init fields */
	state.row_pos = 0;
	state.col_pos = 0;
	state.color = vga::entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	state.buffer = (uint16_t*) VGA_MEMORY_BASE;
	for (size_t row = 0; row < VGA_HEIGHT; row++) {
        state.line_len[row] = 0;
	}

    /* clear screen */
	for (size_t row = 0; row < VGA_HEIGHT; row++) {
        clear_line(row);
	}

    updatecursor(state.col_pos, state.row_pos);

    keyboard::subscribeToInputEvent(handleKeyboardInput);
}

void setcolor(vga::vga_color fg, vga::vga_color bg) 
{
	state.color = vga::entry_color(fg, bg);
}

void putentryat(char c, uint8_t color, size_t x, size_t y) 
{
	const size_t index = y * VGA_WIDTH + x;
	state.buffer[index] = vga::entry(c, color);
}


/* positive is scroll text up */
void scroll(int scrollAmount) {
    if (scrollAmount > 0) {
        for (size_t row = scrollAmount; row < VGA_HEIGHT; row++) {
            state.line_len[row - scrollAmount] = state.line_len[row];
            for (size_t col = 0; col < VGA_WIDTH; col++) {
                const size_t old_index = row * VGA_WIDTH + col;
                const size_t new_index = (row - scrollAmount) * VGA_WIDTH + col;
                state.buffer[new_index] = state.buffer[old_index];
            }
            clear_line(row);
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
            clear_line(row);
        }
    }
}

char getChar(int col, int row) {
    const size_t index = row * VGA_WIDTH + col;
	return vga::extractChar(state.buffer[index]);
}

void deleteChar() {
    if (state.line_len[state.row_pos] == 0) {
        return;
    }

    for (int col = state.col_pos - 1; col < state.line_len[state.row_pos] - 1; col++) {
        char c = getChar(col + 1, state.row_pos);
        putentryat(c, state.color, col, state.row_pos);
    }
    putentryat(' ', state.color, state.line_len[state.row_pos] - 1, state.row_pos);
    state.line_len[state.row_pos]--;
    updatecursor(state.col_pos - 1, state.row_pos);
}

void putchar(char c) 
{
    if (c == '\n') {
        state.line_len[state.row_pos] = state.col_pos;
        state.col_pos = 0;
        if (++state.row_pos == VGA_HEIGHT) {
            state.row_pos = VGA_HEIGHT - 1;
            scroll(1);
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
        putentryat(' ', state.color, state.col_pos, state.row_pos);
    }
    else {
        putentryat(c, state.color, state.col_pos, state.row_pos);
        state.line_len[state.row_pos] = state.col_pos + 1;
        if (++state.col_pos == VGA_WIDTH) {
            state.col_pos = 0;
            if (++state.row_pos == VGA_HEIGHT) {
                state.row_pos = VGA_HEIGHT - 1;
                scroll(1);
            }
        }
    }
    updatecursor(state.col_pos, state.row_pos);
}

void write(const char* data, size_t size) 
{
	for (size_t i = 0; i < size; i++) {
		putchar(data[i]);
    }
}

void writestring(const char* data) 
{
	write(data, strlen(data));
}

}