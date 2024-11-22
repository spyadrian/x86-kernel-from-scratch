#ifndef _KEY_HANDLER_H_
#define _KEY_HANDLER_H_

// --- DEFINE ---

#include <stdint.h>
typedef void (*key_handler_t)(void);

extern key_handler_t const SPECIAL_KEY_HANDLERS[];

// --- FUNCTION ---

/**
 * @brief Handles a generic key press event.
 *
 * If the key corresponds to a special handler, the handler is invoked.
 * Otherwise, the key is printed as a character if it is valid.
 *
 * @param key The key code to handle (0 indicates no key press).
 */
void handle_key(uint16_t key);

/**
 * @brief Handles the Escape (ESC) key press.
 *
 * Triggers a system halt.
 */
void handle_esc(void);

/**
 * @brief Handles the Home key press.
 *
 * Currently does nothing.
 */
void handle_home(void);

/**
 * @brief Handles the Left Arrow key press.
 *
 * Currently does nothing.
 */
void handle_left(void);

/**
 * @brief Handles the Right Arrow key press.
 *
 * Currently does nothing.
 */
void handle_right(void);

/**
 * @brief Handles the Up Arrow key press.
 *
 * Currently does nothing.
 */
void handle_up(void);

/**
 * @brief Handles the Down Arrow key press.
 *
 * Currently does nothing.
 */
void handle_down(void);

/**
 * @brief Handles the End key press.
 *
 * Currently does nothing.
 */
void handle_end(void);

/**
 * @brief Handles the Page Up (PgUp) key press.
 *
 * Currently does nothing.
 */
void handle_pgup(void);

/**
 * @brief Handles the Page Down (PgDn) key press.
 *
 * Currently does nothing.
 */
void handle_pgdown(void);

/**
 * @brief Handles the Insert (Ins) key press.
 *
 * Currently does nothing.
 */
void handle_insert(void);

/**
 * @brief Handles the Delete (Del) key press.
 *
 * Currently does nothing.
 */
void handle_del(void);

/**
 * @brief Handles the F1 key press.
 *
 * Triggers a software interrupt with vector 0.
 */
void handle_f1(void);

/**
 * @brief Handles the F2 key press.
 *
 * Currently does nothing.
 */
void handle_f2(void);

/**
 * @brief Handles the F3 key press.
 *
 * Currently does nothing.
 */
void handle_f3(void);

/**
 * @brief Handles the F4 key press.
 *
 * Triggers a software interrupt with vector 250.
 */
void handle_f4(void);

/**
 * @brief Handles the F5 key press.
 *
 * Triggers a software interrupt with vector 14.
 */
void handle_f5(void);

/**
 * @brief Handles the F6 key press.
 *
 * Currently does nothing.
 */
void handle_f6(void);

/**
 * @brief Handles the F7 key press.
 *
 * Currently does nothing.
 */
void handle_f7(void);

/**
 * @brief Handles the F8 key press.
 *
 * Initiates a delay of 5000 milliseconds (5 seconds).
 */
void handle_f8(void);

/**
 * @brief Handles the F9 key press.
 *
 * Currently does nothing.
 */
void handle_f9(void);

/**
 * @brief Handles the F10 key press.
 *
 * Currently does nothing.
 */
void handle_f10(void);

/**
 * @brief Handles the F11 key press.
 *
 * Currently does nothing.
 */
void handle_f11(void);

/**
 * @brief Handles the F12 key press.
 *
 * Currently does nothing.
 */
void handle_f12(void);

#endif // !_KEY_HANDLER_H_
