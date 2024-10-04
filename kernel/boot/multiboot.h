#ifndef _MULTIBOOT_H_
#define _MULTIBOOT_H_

#include "common/types.h"
#include "multiboot_structs.h"

// --- DEFINE ---

// ...

// --- FUNCTION ---

/**
 * @brief Stores the multiboot info structure.
 *
 * @param _mbi  Pointer to the multiboot info structure to store.
 *
 * This function stores a reference to the multiboot information structure, which contains data passed by the bootloader,
 * such as memory maps, kernel parameters, and other boot-related information.
 */
void multiboot_set_info(multiboot_info_t *_mbi);

/**
 * @brief Retrieves the stored multiboot info structure.
 *
 * @return A pointer to the stored multiboot info structure.
 *
 * This function returns the previously stored multiboot information structure, allowing access to data such as memory maps,
 * bootloader information, and other system-specific details provided at boot time.
 */
multiboot_info_t* multiboot_get_info();

/**
 * @brief Retrieves the amount of available RAM in kilobytes (KB).
 *
 * @return The total available RAM in kilobytes (KB).
 *
 * This function calculates and returns the amount of available physical RAM as reported by the multiboot information structure.
 * It provides the total RAM in KB, which is useful for system initialization and resource management.
 */
uint_t multiboot_get_RAM_in_KB();

#endif // _MULTIBOOT_H_
