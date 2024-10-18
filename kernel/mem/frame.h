#ifndef _FRAME_H_
#define _FRAME_H_

#include <stdint.h>

// --- DEFINE ---

// The hardware supports 3 frame sizes: 4KB, 4MB and 2MB (when PAE is enabled)
// Our kernel only uses 4KB frames.
#define FRAME_SIZE  4096

// Converts a 32-bit address into a frame number
#define ADDR_TO_FRAME_NB(addr) (((uint32_t)addr) / FRAME_SIZE)

// Converts a frame number into a 32-bit address
#define FRAME_NB_TO_ADDR(n) (((uint32_t)n) * FRAME_SIZE)

// Returns the number of frames required to store the given number of bytes
#define FRAME_COUNT(size) ((size + FRAME_SIZE - 1) / FRAME_SIZE)

// --- FUNCTION ---

// Initializes the physical frame subsystem, using the specified amount of physical memory.
void frame_init(uint32_t RAM_in_KB);

// Allocates a frame (4KB) and returns its physical address.
// Returns the physical address of the frame or
// 0xFFFFFFFF if no more frames are available.
// REMARKS:
// The physical address is always aligned to a 4KB boundary.
// The frame's content is always zeroed.
void *frame_alloc();

// Free a frame.
// REMARK: doesn't check whether the frame was previously allocated or not.
void frame_free(void *frame_addr);

// Returns the total number of free frames.
// This can typically be used by a syscall to retrieve the amount of free RAM.
uint32_t frame_total_free();

 #endif
