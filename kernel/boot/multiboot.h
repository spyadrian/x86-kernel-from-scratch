#ifndef _MULTIBOOT_H_
#define _MULTIBOOT_H_

#include "common/types.h"
#include "multiboot_structs.h"

// Stores the multiboot info structure.
void multiboot_set_info(multiboot_info_t *_mbi);

// Retrieves the multiboot info structure.
multiboot_info_t* multiboot_get_info();

// Retrieves the amount of available RAM in KB.
uint_t multiboot_get_RAM_in_KB();

#endif
