#ifndef _FRAME_H_
#define _FRAME_H_

#include <stdint.h>

// --- DEFINE ---

#define FRAME_SIZE  4096  // kernel only uses 4KB pages

// Converts a 32-bit address into a frame number
#define ADDR_TO_FRAME_NB(addr) (((uint32_t)addr) / FRAME_SIZE)

// Converts a frame number into a 32-bit address
#define FRAME_NB_TO_ADDR(n) (((uint32_t)n) * FRAME_SIZE)

// Returns the number of frames required to store the given number of bytes
#define FRAME_COUNT(size) ((size + FRAME_SIZE - 1) / FRAME_SIZE)

// Align Up an address according to a size
#define ALIGN_UP(size, alignment) (((size) + (alignment) - 1) & ~((alignment) - 1))

// --- FUNCTION ---

/**
 * @brief Initializes the physical frame management subsystem.
 *
 * @param RAM_in_KB The total amount of RAM in kilobytes.
 *
 * This function sets up the frame management system for allocating and freeing physical frames.
 * It determines the amount of available physical memory and prepares data structures to track 
 * frame allocation and availability. The amount of memory is passed as the total number of 
 * kilobytes (RAM_in_KB), and the system will manage the memory in 4KB frames.
 */
void frame_init(uint32_t RAM_in_KB);

/**
 * @brief Allocates a single physical frame and returns its address.
 *
 * @return A pointer to the physical address of the allocated frame, or 0xFFFFFFFF if no more 
 * frames are available.
 *
 * This function finds a free 4KB frame in memory, marks it as allocated, and returns its physical 
 * address. If no free frames are available, it returns 0xFFFFFFFF. The returned address is 
 * guaranteed to be 4KB aligned. Additionally, the content of the frame is cleared (zeroed) before 
 * returning.
 */
void *frame_alloc();

/**
 * @brief Frees a previously allocated frame.
 *
 * @param frame_addr The physical address of the frame to be freed.
 *
 * This function takes the physical address of a frame and marks it as available for future 
 * allocations. It does not check whether the frame was previously allocated or not, so calling 
 * this function with an invalid frame address can lead to undefined behavior.
 */
void frame_free(void *frame_addr);

/**
 * @brief Returns the total number of free frames.
 *
 * @return The number of free (available) frames.
 *
 * This function is used to check how many 4KB frames are currently available for allocation. 
 * It can be used, for example, by a syscall to query the system's available memory.
 */
uint32_t frame_total_free();

#endif // _FRAME_H_
