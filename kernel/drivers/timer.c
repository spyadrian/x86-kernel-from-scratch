#include "timer.h"

#include "interrupt/irq.h"

// --- DEFINE ---

#define PIT_COMMAND    0x43
#define PIT_CHANNEL0   0x40
#define PIT_CHANNEL1   0x41
#define PIT_CHANNEL2   0x42

// --- STATIC FUNCTION ---

// Timer interrupt handler
// Update ticks count and display a logo (animated)
static void timer_handler() {

    // TODO
}

// --- FUNCTION ---

void timer_init(uint32_t freq_hz) {

    // TODO
}

uint32_t timer_get_freq() {

    // TODO
}
 
uint32_t timer_get_ticks() {

    // TODO
}

void timer_sleep(uint32_t ms) {

    // TODO
}
