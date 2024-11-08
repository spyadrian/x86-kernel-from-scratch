#include "keyboard.h"

#include <stdint.h>
#include "keymaps/keymap.h"
#include "common/keycodes.h"

// --- DEFINE ---

// Keyboard data and status registers
#define DATA_REG     0x60
#define STATUS_REG   0x64

extern keymap_t *keymap;

// --- STATIC FUNCTION ---

static void keyboard_handler() {

    // TODO
}

// --- FUNCTION ---

void keyb_init() {

    // TODO
}

int keyb_get_key() {

    // TODO
}
