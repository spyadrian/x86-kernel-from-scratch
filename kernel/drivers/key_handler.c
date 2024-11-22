#include "key_handler.h"

#include <stdint.h>

#include "common/colors.h"
#include "x86.h"
#include "timer.h"
#include "drivers/gfx.h"
#include "common/types.h"
#include "common/keycodes.h"

// --- DEFINE ---

key_handler_t const SPECIAL_KEY_HANDLERS[] = {

    [KEY_ESC] = handle_esc,
    [KEY_HOME] = handle_home,
    [KEY_LEFT] = handle_left,
    [KEY_RIGHT] = handle_right,
    [KEY_UP] = handle_up,
    [KEY_DOWN] = handle_down,
    [KEY_END] = handle_end,
    [KEY_PGUP] = handle_pgup,
    [KEY_PGDOWN] = handle_pgdown,
    [KEY_INSERT] = handle_insert,
    [KEY_DEL] = handle_del,
    [KEY_F1] = handle_f1,
    [KEY_F2] = handle_f2,
    [KEY_F3] = handle_f3,
    [KEY_F4] = handle_f4,
    [KEY_F5] = handle_f5,
    [KEY_F6] = handle_f6,
    [KEY_F7] = handle_f7,
    [KEY_F8] = handle_f8,
    [KEY_F9] = handle_f9,
    [KEY_F10] = handle_f10,
    [KEY_F11] = handle_f11,
    [KEY_F12] = handle_f12,
};

// --- FUNCTION ---

void handle_key(uint16_t key) {

    if (key == 0) return;

    key_handler_t handler = SPECIAL_KEY_HANDLERS[key];
    if (handler) {

        handler();
    }
    else {

        char c = (char)key;
        if (c != 0) gfx_printf("%c", c);
    }
}

void handle_esc(void) {

    halt();
}

void handle_home(void) { }

void handle_left(void) { }

void handle_right(void) { }

void handle_up(void) { }

void handle_down(void) { }

void handle_end(void) { }

void handle_pgup(void) { }

void handle_pgdown(void) { }

void handle_insert(void) { }

void handle_del(void) { }

void handle_f1(void) {

    asm volatile("int $0");
}

void handle_f2(void) { }

void handle_f3(void) { }

void handle_f4(void) {

    asm volatile("int $250");
}

void handle_f5(void) {

    asm volatile("int $14");
}

void handle_f6(void) { }

void handle_f7(void) { }

void handle_f8(void) {

    timer_sleep(5000);
}

void handle_f9(void) { }

void handle_f10(void) { }

void handle_f11(void) { }

void handle_f12(void) { }
