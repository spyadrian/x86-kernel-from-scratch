#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include <stdint.h>

// --- DEFINE ---

void keyb_init();

// Returns the key that was pressed or 0 if no key is present in the internal buffer.
// This function never blocks.
uint16_t keyb_get_key();

#endif // _KEYBOARD_H_
