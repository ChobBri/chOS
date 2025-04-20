#include "screen.h"
#include <cstdint>
#include "vga.h"
#include "math.h"

namespace screen {

static int screen_width;
static int screen_height;

void init() {
    using namespace vga;
    write_misc_output_reg(0x63);
    
    write_seq_reg(SEQ_IDX_RST_REG, 0x03);
    write_seq_reg(SEQ_IDX_CLK_MODE_REG, 0x01);
    write_seq_reg(SEQ_IDX_MAP_MASK_REG, 0x0F);
    write_seq_reg(SEQ_IDX_CHAR_MAP_SEL_REG, 0x00);
    write_seq_reg(SEQ_IDX_SEQ_MEM_MODE_REG, 0x0E);

    write_atrb_reg(ATRB_CTRL_IDX_PALET_REG0 , 0x00);
    write_atrb_reg(ATRB_CTRL_IDX_PALET_REG1 , 0x01);
    write_atrb_reg(ATRB_CTRL_IDX_PALET_REG2 , 0x02);
    write_atrb_reg(ATRB_CTRL_IDX_PALET_REG3 , 0x03);
    write_atrb_reg(ATRB_CTRL_IDX_PALET_REG4 , 0x04);
    write_atrb_reg(ATRB_CTRL_IDX_PALET_REG5 , 0x05);
    write_atrb_reg(ATRB_CTRL_IDX_PALET_REG6 , 0x06);
    write_atrb_reg(ATRB_CTRL_IDX_PALET_REG7 , 0x07);
    write_atrb_reg(ATRB_CTRL_IDX_PALET_REG8 , 0x08);
    write_atrb_reg(ATRB_CTRL_IDX_PALET_REG9 , 0x09);
    write_atrb_reg(ATRB_CTRL_IDX_PALET_REG10, 0x0A);
    write_atrb_reg(ATRB_CTRL_IDX_PALET_REG11, 0x0B);
    write_atrb_reg(ATRB_CTRL_IDX_PALET_REG12, 0x0C);
    write_atrb_reg(ATRB_CTRL_IDX_PALET_REG13, 0x0D);
    write_atrb_reg(ATRB_CTRL_IDX_PALET_REG14, 0x0E);
    write_atrb_reg(ATRB_CTRL_IDX_PALET_REG15, 0x0F);

    write_atrb_reg(ATRB_CTRL_IDX_ATRB_MODE_CTRL_REG, 0x41);
    write_atrb_reg(ATRB_CTRL_IDX_OVERSCAN_COLOR_REG, 0x00);
    write_atrb_reg(ATRB_CTRL_IDX_COLOR_PLANE_ENABLE_REG, 0x0F);
    write_atrb_reg(ATRB_CTRL_IDX_HORZ_PXL_PAN_REG, 0x00);
    write_atrb_reg(ATRB_CTRL_IDX_COLOR_SELECT_REG, 0x00);
    


    write_gfx_reg(GFX_IDX_SET_RST_REG, 0x00);
    write_gfx_reg(GFX_IDX_ENABLE_SET_RST_REG, 0x00);
    write_gfx_reg(GFX_IDX_CLR_CMP_REG, 0x00);
    write_gfx_reg(GFX_IDX_DATA_ROT_REG, 0x00);
    write_gfx_reg(GFX_IDX_READ_MAP_SEL_REG, 0x00);
    write_gfx_reg(GFX_IDX_GFX_MODE_REG, 0x40);
    write_gfx_reg(GFX_IDX_MISC_GFX_REG, 0x05);
    write_gfx_reg(GFX_IDX_COLOR_DNT_CARE_REG, 0x0F);
    write_gfx_reg(GFX_IDX_BIT_MASK_REG, 0xFF);

    write_crt_ctrl_reg(CRTC_IDX_END_HORZ_BLANK_REG, read_crt_ctrl_reg(0x03) | 0x80);
    write_crt_ctrl_reg(CRTC_IDX_VERT_RETRACE_END_REG, read_crt_ctrl_reg(0x11) & ~0x80);

    write_crt_ctrl_reg(CRTC_IDX_HORZ_TOTAL_REG, 0x5F);
    write_crt_ctrl_reg(CRTC_IDX_END_HORZ_DISP_REG, 0x4F);
    write_crt_ctrl_reg(CRTC_IDX_START_HORZ_BLANK_REG, 0x50);
    write_crt_ctrl_reg(CRTC_IDX_END_HORZ_BLANK_REG, 0x82);
    write_crt_ctrl_reg(CRTC_IDX_START_HORZ_RETRACE_REG, 0x54);
    write_crt_ctrl_reg(CRTC_IDX_END_HORZ_RETRACE_REG, 0x80);
    write_crt_ctrl_reg(CRTC_IDX_VERT_TOTAL_REG, 0xBF);
    write_crt_ctrl_reg(CRTC_IDX_OVERFLOW_REG, 0x1F);
    write_crt_ctrl_reg(CRTC_IDX_PRESET_ROW_SCAN_REG, 0x00);
    write_crt_ctrl_reg(CRTC_IDX_MAX_SCAN_LINE_REG, 0x41);
    write_crt_ctrl_reg(CRTC_IDX_CURSOR_START_REG, 0x00);
    write_crt_ctrl_reg(CRTC_IDX_CURSOR_END_REG, 0x00);
    write_crt_ctrl_reg(CRTC_IDX_START_ADDR_HIGH_REG, 0x00);
    write_crt_ctrl_reg(CRTC_IDX_START_ADDR_LOW_REG, 0x00);
    write_crt_ctrl_reg(CRTC_IDX_CURSOR_LOC_HIGH_REG, 0x00);
    write_crt_ctrl_reg(CRTC_IDX_CURSOR_LOC_LOW_REG, 0x00);
    write_crt_ctrl_reg(CRTC_IDX_VERT_RETRACE_START_REG, 0x9C);
    write_crt_ctrl_reg(CRTC_IDX_VERT_RETRACE_END_REG, 0x8E);
    write_crt_ctrl_reg(CRTC_IDX_VERT_DISP_END_REG, 0x8F);
    write_crt_ctrl_reg(CRTC_IDX_OFFSET_REG, 0x28);
    write_crt_ctrl_reg(CRTC_IDX_UNDERLINE_LOC_REG, 0x40);
    write_crt_ctrl_reg(CRTC_IDX_START_VERT_BLANK_REG, 0x96);
    write_crt_ctrl_reg(CRTC_IDX_END_VERT_BLANK_REG, 0xB9);
    write_crt_ctrl_reg(CRTC_IDX_CRTC_MODE_CTRL_REG, 0xA3);
    write_crt_ctrl_reg(CRTC_IDX_LINE_COMP_REG, 0xFF);

    /* load color palette */
    for (int i = 0; i < 256; i++) {
        uint8_t r = (i & 0x7) * 9;
        uint8_t g = ((i >> 3) & 0x3) * 21;
        uint8_t b = ((i >> 5) & 0x7) * 9;
        
        write_color_reg(i, r, g, b);
    }

    screen_width = 320;
    screen_height = 200;
}

int width() {
    return screen_width;
}

int height() {
    return screen_height;
}

void putpixel(int x, int y, uint8_t r, uint8_t g, uint8_t b) {
    r = r / 32;
    g = g / 64;
    b = b / 32;
    ((uint8_t*)0xA0000)[y * screen_width + x] = b * (8 * 4)  + g * 8 + r;
}

void drawline(int x0, int y0, int x1, int y1, uint8_t r, uint8_t g, uint8_t b) {
    r = r / 32;
    g = g / 64;
    b = b / 32;

    if (x0 > x1) {
        int tmp = x0;
        x0 = x1;
        x1 = tmp;
        tmp = y0;
        y0 = y1;
        y1 = tmp;
    }

    int dx = x1 - x0;
    int dy = y1 - y0;

    if (dx == 0) {
        for (int y = y0; y <= y1; y++) {
            ((uint8_t*)0xA0000)[y * screen_width + x0] = b * (8 * 4)  + g * 8 + r;
        }
        return;
    }
    
    float m = dy / (float) dx;

    int xend = min(x1, screen_width - 1);
    
    for (int x = x0; x <= xend; x++) {
        int y = round(m * (x - x0)) + y0;
        ((uint8_t*)0xA0000)[y * screen_width + x] = b * (8 * 4)  + g * 8 + r;
    }
}

}
