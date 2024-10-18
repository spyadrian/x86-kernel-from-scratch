#include "frame.h"

#include <stdbool.h>
#include <stdint.h>
#include "drivers/module.h"

// --- DEFINE ---

#define BITMAP_SIZE 131072  // 4GB (IA-32) = 1048576B pages => 1048576 / 8 = 131072 bits

// frame 0 represents physical addresses [0,4095]
// frame 33 represents physical addresses [33*4096,33*4096+4095] = [135168,139263]
static uint8_t frame_bitmap[BITMAP_SIZE];   // each frame is represented with 1 bit, 0 is free, 1 is used

static uint32_t frame_usable_start;         // start of usable frame (end module)
static uint32_t frame_usable_end;           // end of usable frame (end ram)

// --- STATIC ---

/**
 * @brief Marks a frame as used in the frame bitmap.
 *
 * @param frame The frame number to mark as used.
 *
 * This function sets the bit corresponding to the specified frame in the frame_bitmap to 1,
 * indicating that the frame is now allocated and unavailable for future allocation.
 */
static void frame_mark_used(uint32_t frame) {

    uint32_t id = frame / 8;
    uint32_t offset = frame % 8;

    frame_bitmap[id] |= 1 << offset;
}

/**
 * @brief Marks a frame as free in the frame bitmap.
 *
 * @param frame The frame number to mark as free.
 *
 * This function clears the bit corresponding to the specified frame in the frame_bitmap to 0,
 * indicating that the frame is now free and available for allocation.
 */
static void frame_mark_free(uint32_t frame) {

    uint32_t id = frame / 8;
    uint32_t offset = frame % 8;

    frame_bitmap[id] &= ~(1 << offset);
}

/**
 * @brief Checks if a frame is free.
 *
 * @param frame The frame number to check.
 * @return true if the frame is free, false if the frame is used.
 *
 * This function reads the bit corresponding to the frame in the frame_bitmap. If the bit is 0,
 * the frame is free; if it's 1, the frame is used.
 */
static bool is_frame_free(uint32_t frame) {

    uint32_t id = frame / 8;
    uint32_t offset = frame % 8;

    return !((frame_bitmap[id] >> offset) & 1);
}

// --- FUNCTION ---

void frame_init(uint32_t ram_kb) {

    frame_usable_start = ADDR_TO_FRAME_NB(ALIGN_UP((uint32_t)module_last_addr(), FRAME_SIZE));
    frame_usable_end = ram_kb;

    // set as used everything after the RAM size
    for (uint32_t i = frame_usable_end; i < BITMAP_SIZE; i++)
        frame_mark_used(i);

    // set as free the part between the last module and the end of the ram
    for (uint32_t i = frame_usable_start; i < frame_usable_end; i++)
        frame_mark_free(i);

    // set as used everything below the last module
    for (uint32_t i = 0; i < frame_usable_start; i++)
        frame_mark_used(i);
}

void *frame_alloc() {

    uint32_t frame_num = 0;

    // get first free frame
    for (uint32_t i = frame_usable_start; i < frame_usable_end; i++) {
        if (is_frame_free(i)) {

            frame_num = i;
            break;
        }
    }

    // check if the frame is realy free
    if (!is_frame_free(frame_num))
        return (void *)0xFFFFFFFF;

    frame_mark_used(frame_num);

    // get and clear frame
    uint8_t *frame = (uint8_t *)FRAME_NB_TO_ADDR(frame_num);
    for (uint32_t i = 0; i < FRAME_SIZE; i++)
        frame[i] = 0;

    return (void *)frame;
}

void frame_free(void *frame_addr) {

    frame_mark_free(ADDR_TO_FRAME_NB(frame_addr));
}

uint32_t frame_total_free() {

    uint32_t count = 0;

    for (uint32_t i = 0; i < BITMAP_SIZE; i++) {
        if (is_frame_free(i))
            count++;
    }

    return count;
}
