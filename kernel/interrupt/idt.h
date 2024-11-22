#ifndef _IDT_H_
#define _IDT_H_

// --- DEFINE ---

/**
 * @brief Initializes the Interrupt Descriptor Table (IDT).
 *
 * Sets up the IDT with entries for exceptions and hardware interrupts,
 * and loads it into the CPU. This is a crucial step for handling both 
 * processor exceptions and external device interrupts.
 */
void idt_init();

#endif // _IDT_H_
