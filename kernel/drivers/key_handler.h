#ifndef _KEY_HANDLER_H_
#define _KEY_HANDLER_H_

// --- DEFINE ---

#include <stdint.h>
typedef void (*key_handler_t)(void);

extern key_handler_t const SPECIAL_KEY_HANDLERS[];

// --- FUNCTION ---

void handle_key(uint16_t key);

void handle_esc(void);

void handle_home(void);

void handle_left(void);

void handle_right(void);

void handle_up(void);

void handle_down(void);

void handle_end(void);

void handle_pgup(void);

void handle_pgdown(void);

void handle_insert(void);

void handle_del(void);

void handle_f1(void);

void handle_f2(void);

void handle_f3(void);

void handle_f4(void);

void handle_f5(void);

void handle_f6(void);

void handle_f7(void);

void handle_f8(void);

void handle_f9(void);

void handle_f10(void);

void handle_f11(void);

void handle_f12(void);

#endif // !_KEY_HANDLER_H_
