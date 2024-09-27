#include "gfx.h"

#include "common/mem.h"

// --- DEFINE ---

// ...

static multiboot_info_t *_mbi;

// --- STATIC ---

static uint16_t *pixel_to_addr(uint32_t x, uint32_t y) {

    uint16_t *addr = ((uint16_t *)(_mbi->framebuffer_addr)) + (y * (uint32_t)_mbi->framebuffer_pitch / 2) + x;
    return addr;
}

// --- FUNCTION ---

void gfx_init() {

    _mbi = multiboot_get_info();
}

void gfx_set_pixel(uint32_t x, uint32_t y, uint16_t color) {

    uint16_t *p = pixel_to_addr(x, y);
    *p = color;
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

// void gfx_printf(uint16_t color, uint16_t back_color, char *str, ...);

