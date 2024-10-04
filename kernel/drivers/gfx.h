#ifndef _GFX_H_
#define _GFX_H_

#include <stdint.h>
#include "common/colors.h"

// --- DEFINE ---

#define DEFAULT_FONT_COLOR        WHITE
#define DEFAULT_FONT_BACK_COLOR   BLACK

// --- FUNCTION ---

/**
 * @brief Initializes the graphics system.
 */
void gfx_init();

/**
 * @brief Sets the color of a specific pixel on the screen.
 *
 * @param x      The x-coordinate of the pixel.
 * @param y      The y-coordinate of the pixel.
 * @param color  The color to set the pixel to.
 */
void gfx_set_pixel(uint16_t x, uint16_t y, uint16_t color);

/**
 * @brief Sets the current font and background colors for text rendering.
 *
 * @param color       The color of the text.
 * @param back_color  The background color behind the text.
 */
void gfx_set_font_color(uint16_t color, uint16_t back_color);

/**
 * @brief Sets the cursor position for text rendering.
 *
 * @param x  The x-coordinate for the cursor.
 * @param y  The y-coordinate for the cursor.
 */
void gfx_set_cursor(uint16_t x, uint16_t y);

/**
 * @brief Retrieves the current x-coordinate of the cursor.
 */
uint16_t gfx_get_cursor_x();

/**
 * @brief Retrieves the current y-coordinate of the cursor.
 */
uint16_t gfx_get_cursor_y();

/**
 * @brief Fills the entire screen with a single color.
 */
void gfx_fill_screen(uint16_t color);

/**
 * @brief Prints formatted text with specified foreground and background colors.
 *
 * @param color        The color of the text.
 * @param back_color   The background color behind the text.
 * @param str          The format string (similar to printf).
 * @param ...          Additional arguments for formatting.
 */
void gfx_printf_colored(uint16_t color, uint16_t back_color, char *str, ...);

/**
 * @brief Prints formatted text using the current font and background colors.
 *
 * @param str  The format string (similar to printf).
 * @param ...  Additional arguments for formatting.
 */
void gfx_printf(char *str, ...);

/**
 * @brief Renders an image on the screen at a specified position.
 *
 * @param img     Pointer to the image data.
 * @param width   The width of the image in pixels.
 * @param height  The height of the image in pixels.
 * @param x       The x-coordinate where the image will be placed.
 * @param y       The y-coordinate where the image will be placed.
 */
void gfx_render_image(uint8_t *img, uint16_t width, uint16_t height, uint16_t x, uint16_t y);

#endif // _GFX_H_
