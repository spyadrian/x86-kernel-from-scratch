#include "syscall.h"

void main() {

    uint16_t *fb = (uint16_t *)0xFE000000;

    for (int i = 0; i < 640 * 240; i++) {
        fb[i] = 0xFFFF; // white
    }

    syscall(SYSCALL_TIMER_SLEEP, 1000, 0, 0, 0);

    for (int i = 0; i < 640 * 240; i++) {
        fb[i] = 0x00FF;
    }
}
