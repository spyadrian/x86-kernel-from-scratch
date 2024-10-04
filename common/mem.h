#ifndef _MEM_COMMON_H_
#define _MEM_COMMON_H_

#include "types.h"

// --- DEFINE ---

// ...

// --- FUNCTION ---

/**
 * @brief Fills a block of memory with a specified byte value.
 *
 * @param dst    Pointer to the memory area to be filled.
 * @param value  The byte value to fill the memory with.
 * @param count  The number of bytes to fill.
 */
extern void memset(void *dst, uint8_t value, uint_t count);

/**
 * @brief Fills a block of memory with a specified dword (32-bit) value.
 *
 * @param dst    Pointer to the memory area to be filled.
 * @param value  The dword (32-bit) value to fill the memory with.
 * @param count  The number of dwords to fill.
 */
void memsetdw(void *dst, uint32_t value, uint_t count);

/**
 * @brief Copies a block of memory from a source to a destination.
 *
 * @param dst    Pointer to the destination memory area.
 * @param src    Pointer to the source memory area.
 * @param count  The number of bytes to copy.
 */
void memcpy(void *dst, void *src, uint_t count);

/**
 * @brief Copies a block of memory using dword (32-bit) values from source to destination.
 *
 * @param dst    Pointer to the destination memory area.
 * @param src    Pointer to the source memory area.
 * @param count  The number of dwords to copy.
 */
void memcpydw(void *dst, void *src, uint_t count);

#endif // _MEM_COMMON_H_
