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
 * @brief Scrolls the graphical display vertically by a specified number of rows.
 *
 * @param nb_row  The number of rows to scroll. A positive value scrolls the display up, 
 *                while a negative value scrolls the display down.
 *
 * This function moves the content of the graphical display by a given number of rows.
 * When scrolling up, lines at the top are removed, and empty space is added at the bottom.
 * When scrolling down, lines at the bottom are removed, and empty space is added at the top.
 */
void gfx_scroll_v(int16_t nb_row);

/**
 * @brief Clears a specific line on the graphical display and fills it with a specified color.
 *
 * @param line    The line number to clear. The line index starts at 0 for the topmost line.
 * @param color   The color used to fill the cleared line.
 *
 * This function removes any content on the specified line of the graphical display and fills
 * the entire line with a uniform color, typically used to reset or refresh a single line of the display.
 */
void gfx_clear_line(uint16_t line, uint16_t color);

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
