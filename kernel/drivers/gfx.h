#ifndef _GFX_H_
#define _GFX_H_

#include "common/types.h"
#include "boot/multiboot.h"

// --- DEFINE ---

// ...

// --- FUNCTION ---

void gfx_init();

void gfx_set_pixel(uint32_t x, uint32_t y, uint16_t color);

void gfx_fill_screen(uint16_t color);

void gfx_printf(uint16_t color, uint16_t back_color, char *str, ...);

#endif
