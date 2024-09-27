#ifndef _GFX_H_
#define _GFX_H_

#include "common/types.h"
#include "common/colors.h"
#include "boot/multiboot.h"

// --- DEFINE ---

#define DEFAULT_FONT_COLOR        WHITE
#define DEFAULT_FONT_BACK_COLOR   BLACK

// --- FUNCTION ---

void gfx_init();

void gfx_set_pixel(uint16_t x, uint16_t y, uint16_t color);

void gfx_set_font_color(uint16_t color, uint16_t back_color);

void gfx_set_cursor(uint16_t x, uint16_t y);

void gfx_fill_screen(uint16_t color);

void gfx_printf_colored(uint16_t color, uint16_t back_color, char *str, ...);

void gfx_printf(char *str, ...);

#endif
