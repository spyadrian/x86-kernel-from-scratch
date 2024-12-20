#include "ulibc.h"

void main() {

    uint16_t *fb = (uint16_t *)0xFE000000;

    for (int i = 0; i < 640 * 240; i++) {
        fb[i] = 0xFFFF ; // white
    }
}
