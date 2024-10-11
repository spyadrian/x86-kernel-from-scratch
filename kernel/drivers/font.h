#ifndef _FONT_H_
#define _FONT_H_

#include <stdint.h>

// --- DEFINE ---

#define FONT_HEIGHT      16
#define FONT_WIDTH        8
#define CHARS_COUNT     256

// --- FUNCTION ---

/**
 * @brief A font array representing 256 characters, each 8x16 pixels.
 *
 * This array stores the bitmap representation of each character in a
 * 256-character set. Each character is 8 pixels wide and 16 pixels tall.
 * The font data is organized as an array of 256 characters, where each character
 * takes up 16 bytes (one byte per row, 8 pixels per byte).
 */
extern uint8_t font_8x16[CHARS_COUNT * FONT_HEIGHT];

#endif // _FONT_H_
