#include "gfx.h"

#include <stdarg.h>
#include <stdint.h>
#include "common/colors.h"
#include "font.h"
#include "common/stdio.h"
#include "boot/multiboot.h"

// --- DEFINE ---

#define ABS(x) (((x) < 0) ? -(x) : (x))
#define SIGN(x) (((x) == 0) ? 0 : (((x) < 0) ? -1 : 1))

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

void gfx_set_pixel(uint16_t x, uint16_t y, uint16_t color) {

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

uint16_t gfx_get_cursor_x() {

    return cursor.x * FONT_WIDTH;
}

uint16_t gfx_get_cursor_y() {

    return cursor.y * FONT_HEIGHT;
}

void gfx_scroll_v(int16_t nb_row) {

    uint16_t line = (nb_row > 0) ? 0 : _mbi->framebuffer_height - 1;
    uint16_t *p = pixel_to_addr(0, line);
    uint8_t off_screen_width = _mbi->framebuffer_pitch / 2 - _mbi->framebuffer_width;

    // scroll verticaly
    for (uint32_t i = ABS(nb_row); i < (uint32_t)_mbi->framebuffer_height; i++) {
        for (uint32_t j = 0; j < (uint32_t)_mbi->framebuffer_width; j++) {

            *p = p[_mbi->framebuffer_width * nb_row];
            p += SIGN(nb_row);
        }

        p += off_screen_width * SIGN(nb_row);
    }

    // clear last line
    for (uint32_t i = 0; i < FONT_HEIGHT; i++) {
        for (uint32_t j = 0; j < (uint32_t)_mbi->framebuffer_width; j++) {

            *p = BLACK;
            p += SIGN(nb_row);
        }

        p += off_screen_width * SIGN(nb_row);
    }
}

void gfx_clear_line(uint16_t line, uint16_t color) {

    uint16_t *p = pixel_to_addr(0, line);
    uint16_t off_screen_width = _mbi->framebuffer_pitch / 2 - _mbi->framebuffer_width;

    for (uint32_t i = 0; i < FONT_HEIGHT; i++) {
        for (uint32_t j = 0; j < (uint32_t)_mbi->framebuffer_width; j++) {

            *p++ = color;
        }

        p += off_screen_width;
    }
}

void gfx_fill_screen(uint16_t color) {

    uint16_t *p = pixel_to_addr(0, 0);
    uint16_t off_screen_width = _mbi->framebuffer_pitch / 2 - _mbi->framebuffer_width;

    for (uint32_t i = 0; i < (uint32_t)_mbi->framebuffer_height; i++) {
        for (uint32_t j = 0; j < (uint32_t)_mbi->framebuffer_width; j++) {

            *p++ = color;
        }

        p += off_screen_width;
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

void gfx_render_image(uint8_t *img, uint16_t width, uint16_t height, uint16_t x, uint16_t y) {

    uint16_t lim_x = (x + width > _mbi->framebuffer_width) ? _mbi->framebuffer_width - x : width;
    uint16_t lim_y = (y + height > _mbi->framebuffer_height) ? _mbi->framebuffer_height - y : height;

    for (uint16_t i = 0; i < lim_y; i++) {

        for (uint16_t j = 0; j < lim_x; j++) {
            uint8_t src[3] = {0};
            src[0] = *img++;
            src[1] = *img++;
            src[2] = *img++;

            uint16_t color = RGB(src[0], src[1], src[2]);
            gfx_set_pixel(x + j, y + i, color);
        }

        img += width - lim_x;
    }
}
