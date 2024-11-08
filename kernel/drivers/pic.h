#ifndef _PIC_H_
#define _PIC_H_

#include <stdint.h>

// --- FUNCTION ---

// Initialize both PICs by mapping IRQs 0-15 to interrupts 32-47.
void pic_init();

// Send an EOI to the PICs given which IRQ was handled.
void pic_eoi(int32_t irq);

#endif // _PIC_H_
