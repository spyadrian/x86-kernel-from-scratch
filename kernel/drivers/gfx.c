#include "gfx.h"

#include "font.h"
#include "common/stdio.h"

// --- DEFINE ---

#define BUFFER_SIZE 1024

typedef struct {

    uint16_t x;
    uint16_t y;
} cursor_t;

static multiboot_info_t *_mbi;
static cursor_t cursor;

static uint16_t font_color = DEFAULT_FONT_COLOR;
static uint16_t font_back_color = DEFAULT_FONT_BACK_COLOR;

// --- STATIC ---

static uint16_t *pixel_to_addr(uint32_t x, uint32_t y) {

    uint16_t *addr = ((uint16_t *)((uint32_t)(_mbi->framebuffer_addr))) + (y * (uint32_t)_mbi->framebuffer_pitch / 2) + x;
    return addr;
}

static void fix_cursor() {

    if (cursor.x * FONT_WIDTH > _mbi->framebuffer_width - FONT_WIDTH) {

        cursor = (cursor_t){0, cursor.y + 1};
    }

    if (cursor.y * FONT_HEIGHT > _mbi->framebuffer_height - FONT_HEIGHT) {

        cursor = (cursor_t){0, 0};
    }
}

static void print_char_colored(uint16_t color, uint16_t back_color, char c) {

    int idx = FONT_HEIGHT * c;

    if (c == '\n') {  // special char

        cursor = (cursor_t){0, cursor.y + 1};
    }
    else {            // normal char

        for (int i = 0; i < FONT_HEIGHT; i++) {

            uint16_t *p = pixel_to_addr(cursor.x * FONT_WIDTH, cursor.y * FONT_HEIGHT + i);
            for (int j = 0; j < FONT_WIDTH; j++) {

                // write one pixel of a line of the char, according to his value (0 or 1)
                *p++ = ((font_8x16[idx] >> (FONT_WIDTH - 1 - j)) & 1) ? color : back_color;
            }

            // increment index to go to the second line
            idx++;
        }

        cursor.x += 1;
    }

    fix_cursor();
}

static void vprintf_colored(uint16_t color, uint16_t back_color, char *str, va_list args) {

    char dest[BUFFER_SIZE];
    int16_t len = vsnprintf(dest, BUFFER_SIZE, str, args);

    for (int16_t i = 0; i < len; i++) {

        print_char_colored(color, back_color, dest[i]);
    }
}

// --- FUNCTION ---

void gfx_init() {

    _mbi = multiboot_get_info();

    cursor = (cursor_t){0 ,0};
}

void gfx_set_pixel(uint32_t x, uint32_t y, uint16_t color) {

    uint16_t *p = pixel_to_addr(x, y);
    *p = color;
}

void gfx_set_font_color(uint16_t color, uint16_t back_color) {

    font_color = color;
    font_back_color = back_color;
}

void gfx_set_cursor(uint16_t x, uint16_t y) {

    cursor = (cursor_t){x, y};
    fix_cursor();
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

void gfx_printf_colored(uint16_t color, uint16_t back_color, char *str, ...) {

    va_list args;
    va_start(args, str);

    vprintf_colored(color, back_color, str, args);

    va_end(args);
}

void gfx_printf(char *str, ...) {

    va_list args;
    va_start(args, str);

    vprintf_colored(font_color, font_back_color, str, args);

    va_end(args);
}
