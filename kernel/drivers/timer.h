#ifndef _TIMER_H_
#define _TIMER_H_

#include <stdint.h>

// --- DEFINE ---

#define FREQ_MIN 18
#define FREQ_MAX 1193182

// --- FUNCTION ---

/**
 * @brief Initializes the system timer with a specified frequency.
 *
 * Sets up the timer to operate at a desired frequency (in Hz). If the specified frequency
 * is outside the supported range, the system will halt. To check the actual frequency, 
 * use `timer_get_freq()`.
 *
 * @param freq_hz Desired timer frequency in Hertz.
 */
void timer_init(uint32_t freq_hz);

/**
 * @brief Retrieves the number of timer ticks since the system boot.
 *
 * Returns the total number of ticks counted by the timer. This value can be used
 * to measure elapsed time since the system started.
 *
 * @return Total number of ticks since boot.
 */
uint32_t timer_get_ticks();

/**
 * @brief Causes the system to sleep (actively) for a specified duration.
 *
 * This function uses the timer to implement a blocking delay. During this time,
 * the CPU remains active in a busy-wait loop.
 *
 * @param ms Time to sleep in milliseconds.
 */
void timer_sleep(uint32_t ms);

/**
 * @brief Returns the current operating frequency of the timer.
 *
 * Retrieves the frequency of the timer in Hertz, as set by `timer_init()`.
 *
 * @return Timer frequency in Hertz.
 */
uint32_t timer_get_freq();

#endif // _TIMER_H_
