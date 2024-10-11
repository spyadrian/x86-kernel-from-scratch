#include "frame.h"

#include <stdbool.h>

// --- DEFINE ---

// IA-32 can handle an address space of 4GB.
// 4GB = 1048576 pages (4KB each).
// If we represent each frame with 1 bit, we need 1048576 bits = 128KB (131072 bytes).
static uint8_t frame_bitmap[131072];

// --- STATIC ---

// Marks a frame as allocated.
// frame 0 represents physical addresses [0,4095]
// frame 33 represents physical addresses [33*4096,33*4096+4095] = [135168,139263]
static void frame_mark_used(uint32_t frame) {
    // TODO
}

// Marks a frame as free/available.
static void frame_mark_free(uint32_t frame) {
    // TODO
}

static bool is_frame_free(uint32_t frame) {
    // TODO
}

// --- FUNCTION ---

void *frame_alloc() {
    // TODO
}

void frame_free(void *frame_addr) {
    // TODO
}

uint32_t frame_total_free() {
    // TODO
}

void frame_init(uint32_t RAM_in_KB) {
    // TODO
}
