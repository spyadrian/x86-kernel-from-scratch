#ifndef _PMIO_H_
#define _PMIO_H_

#include <stdint.h>

// --- FUNCTION ---

// Write a 8-bit data to the specified port
void outb(uint16_t port, uint8_t data);

// Read a 8-bit data from the specified port
uint8_t inb(uint16_t port);

// Write a 16-bit data to the specified port
void outw(uint16_t port, uint16_t data);

// Read a 16-bit data from the specified port
uint16_t inw(uint16_t port);

#endif
