#include "common/types.h"
#include "timer.h"
#include "interrupt/irq.h"

#define PIT_COMMAND    0x43
#define PIT_CHANNEL0   0x40
#define PIT_CHANNEL1   0x41
#define PIT_CHANNEL2   0x42

// Timer interrupt handler
// Update ticks count and display a logo (animated)
static void timer_handler() {
    // TODO
}

void timer_init(uint_t freq_hz) {
    // TODO
}

uint_t timer_get_freq() {
    // TODO
}
 
uint_t timer_get_ticks() {
    // TODO
}

void timer_sleep(uint_t ms) {
    // TODO
}
