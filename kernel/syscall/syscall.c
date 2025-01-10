#include "syscall.h"

#include <stdint.h>

#include "common/types.h"
#include "common/syscall_nb.h"

#include "drivers/gfx.h"
#include "drivers/timer.h"
#include "drivers/keyboard.h"
#include "task/task.h"


static int sys_get_key(uint32_t arg1) {

    int *key = (int *)arg1;

    *key = keyboard_get_key();

    return 0;
}

static int sys_timer_info(uint32_t arg1, uint32_t arg2) {

    uint_t *freq = (uint_t *)arg1;
    uint_t *ticks = (uint_t *)arg2;

    *freq = timer_get_freq();
    *ticks = timer_get_ticks();

    return 0;
}

static int sys_timer_sleep(uint32_t ms) {

    timer_sleep(ms);
    return 0;
}

static int sys_set_pixel(uint32_t x, uint32_t y, uint32_t color) {

    gfx_set_pixel(x, y, color);
    return 0;
}

static int sys_task_exec(uint32_t arg1) {

    char *name = (char *)arg1;

    return task_exec(name);
}

// Called by the assembly function: _syscall_handler
int syscall_handler(syscall_t nb, uint32_t arg1, uint32_t arg2, uint32_t arg3, uint32_t arg4) {

    switch (nb) {

        case SYSCALL_KEY:
            return sys_get_key(arg1);

        case SYSCALL_TIMER_INFO:
            return sys_timer_info(arg1, arg2);

        case SYSCALL_TIMER_SLEEP:
            return sys_timer_sleep(arg1);

        case SYSCALL_SETPIXEL:
            return sys_set_pixel(arg1, arg2, arg2);

        case SYSCALL_TASK_EXEC:
            return sys_task_exec(arg1);

        case __SYSCALL_END__:
        default:
            return -1;
    }
}
