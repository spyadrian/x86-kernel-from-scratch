#include "timer.h"

#include <stdint.h>

#include "common/colors.h"
#include "drivers/gfx.h"
#include "interrupt/irq.h"
#include "pmio/pmio.h"
#include "x86.h"

// --- DEFINE ---

#define PIT_COMMAND 0x43
#define PIT_CHANNEL0 0x40
#define PIT_CHANNEL1 0x41
#define PIT_CHANNEL2 0x42

#define PEPE_NUMBER_FRAME   6
#define PEPE_1_MODULE_IDX   2

static uint32_t _freq_hz;
static uint32_t ticks_count;
static uint32_t free_ticks_count;

static uint8_t pepe_the_frog_idx;
static bool update_pepe;

// --- STATIC FUNCTION ---

// Timer interrupt handler
static void timer_handler() {

    ticks_count++;
    free_ticks_count++;

    // render icon
    gfx_render_image_by_index(PEPE_1_MODULE_IDX + pepe_the_frog_idx, 570, 0);

    if (update_pepe)
        pepe_the_frog_idx = (pepe_the_frog_idx + 1) % PEPE_NUMBER_FRAME;

    update_pepe = !update_pepe;
}

// --- FUNCTION ---

void timer_init(uint32_t freq_hz) {

    pepe_the_frog_idx = 0;

    // set freq
    if (freq_hz < FREQ_MIN || freq_hz > FREQ_MAX)
        halt();

    _freq_hz = freq_hz;
    uint16_t divider = FREQ_MAX / freq_hz;

    outb(PIT_COMMAND, 0x36);
    outb(PIT_CHANNEL0, divider & 0xF);
    outb(PIT_CHANNEL0, divider >> 8);

    // install handler
    ticks_count = 0;

    handler_t hd = {timer_handler, "IRQ_TIMER"};
    irq_install_handler(IRQ_TIMER, hd);
}

uint32_t timer_get_freq() { return _freq_hz; }

uint32_t timer_get_ticks() { return ticks_count; }

void timer_sleep(uint32_t ms) {

    free_ticks_count = 0;
    while (free_ticks_count < ms * _freq_hz / 1000);
}
