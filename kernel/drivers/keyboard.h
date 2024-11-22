#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include <stdint.h>

// --- DEFINE ---

/**
 * @brief Initializes the keyboard driver and sets up the keyboard interrupt handler.
 *
 * This function prepares the keyboard buffer and installs the interrupt handler
 * for processing keyboard input.
 */
void keyboard_init();

/**
 * @brief Retrieves the next key press from the keyboard buffer (FIFO).
 *
 * This function returns the recently pressed key from the internal buffer (FIFO).
 * If the buffer is empty, it returns 0. This function is non-blocking.
 *
 * @return The keycode of the pressed key, or 0 if no key is present in the buffer.
 */
uint16_t keyboard_get_key();

#endif // _KEYBOARD_H_
