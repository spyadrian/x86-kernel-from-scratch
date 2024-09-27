#include "gfx.h"

#include "font.h"
#include "common/stdio.h"

// --- DEFINE ---

#define BUFFER_SIZE 1024

static multiboot_info_t *_mbi;
static uint16_t font_color = DEFAULT_FONT_COLOR;
static uint16_t font_back_color = DEFAULT_FONT_BACK_COLOR;

// --- STATIC ---

static uint16_t *pixel_to_addr(uint32_t x, uint32_t y) {

    uint16_t *addr = ((uint16_t *)((uint32_t)(_mbi->framebuffer_addr))) + (y * (uint32_t)_mbi->framebuffer_pitch / 2) + x;
    return addr;
}

static void print_char_colored(uint32_t x, uint32_t y, uint16_t color, uint16_t back_color, char c) {

    int idx = FONT_HEIGHT * c;

    for (int i = 0; i < FONT_HEIGHT; i++) {

        uint16_t *p = pixel_to_addr(x * FONT_WIDTH, y * FONT_HEIGHT + i);
        for (int j = 0; j < FONT_WIDTH; j++) {

            // write one pixel of a line of the char, according to his value (0 or 1)
            *p++ = ((font_8x16[idx] >> (FONT_WIDTH - 1 - j)) & 1) ? color : back_color;
        }

        // increment index to go to the second line
        idx++;
    }
}

static void vprintf_colored(uint32_t x, uint32_t y, uint16_t color, uint16_t back_color, char *str, va_list args) {

    char dest[BUFFER_SIZE];
    int16_t len = vsnprintf(dest, BUFFER_SIZE, str, args);

    for (int16_t i = 0; i < len; i++) {

        print_char_colored(x, y, color, back_color, dest[i]);
        x++;
    }
}

// --- FUNCTION ---

void gfx_init() {

    _mbi = multiboot_get_info();
}

void gfx_set_pixel(uint32_t x, uint32_t y, uint16_t color) {

    uint16_t *p = pixel_to_addr(x, y);
    *p = color;
}

void gfx_set_font_color(uint16_t color, uint16_t back_color) {

    font_color = color;
    font_back_color = back_color;
}

void gfx_fill_screen(uint16_t color) {

    uint16_t *p = pixel_to_addr(0, 0);
    uint8_t off_screen_width = _mbi->framebuffer_pitch - _mbi->framebuffer_width;

    for (uint32_t i = 0; i < (uint32_t)_mbi->framebuffer_height; i++) {
        for (uint32_t j = 0; j < (uint32_t)_mbi->framebuffer_width; j++) {

            *p++ = color;
        }

        *p += off_screen_width;
    }
}

void gfx_printf_colored(uint32_t x, uint32_t y, uint16_t color, uint16_t back_color, char *str, ...) {

    va_list args;
    va_start(args, str);

    vprintf_colored(x, y, color, back_color, str, args);

    va_end(args);
}

void gfx_printf(uint32_t x, uint32_t y, char *str, ...) {

    va_list args;
    va_start(args, str);

    vprintf_colored(x, y, font_color, font_back_color, str, args);

    va_end(args);
}
