#include "keyboard.h"

#include <stdint.h>

#include "x86.h"
#include "pmio/pmio.h"
#include "common/mem.h"
#include "interrupt/irq.h"
#include "keymaps/keymap.h"
#include "common/keycodes.h"

// --- DEFINE ---

#define DATA_REG     0x60
#define STATUS_REG   0x64

#define BUFFER_SIZE 512

extern keymap_t *keymap;

static uint16_t buffer[BUFFER_SIZE];
static uint32_t buffer_idx;

static bool shifted;

// --- STATIC FUNCTION ---

static void keyboard_handler() {

    // check buffer capacity
    if (buffer_idx >= BUFFER_SIZE) halt();

    // get data
    // while (inb(STATUS_REG) != 1);

    uint8_t data = inb(DATA_REG);
    bool pressed = (data >> 7 == 0);
    uint8_t scancode = data & 0x7F;

    // handle keys
    switch (scancode) {

        case KEY_SHIFT_LEFT:
        case KEY_SHIFT_RIGHT:

            shifted = pressed;
        break;

        default:

            if (pressed) {

                uint16_t *keymap_type = keymap->normal;
                if (shifted)
                    keymap_type = keymap->shift;

                buffer[buffer_idx++] = keymap_type[scancode];
            }
        break;
    }
}

// --- FUNCTION ---

void keyboard_init() {

    shifted = false;

    memset(buffer, 0, sizeof(buffer));
    buffer_idx = 0;

    // install handler
    handler_t hd = {keyboard_handler, "IRQ_KEYBOARD"};
    irq_install_handler(IRQ_KEYBOARD, hd);
}

uint16_t keyboard_get_key() {

    if (buffer_idx == 0)
        return 0;

    return buffer[--buffer_idx];
}
