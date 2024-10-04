#ifndef _MODULE_H_
#define _MODULE_H_

#include <stdint.h>

// --- DEFINE ---

// ...

// --- FUNCTION ---

void module_init();

uint32_t module_count();

void *module_get_addr(uint16_t idx);

uint32_t module_size(uint16_t idx);

char *module_cmdline(uint16_t idx);

void module_get_arg(uint16_t idx, uint8_t nb_arg, char *arg);

void *module_last_addr();

#endif
