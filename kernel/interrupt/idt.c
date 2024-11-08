#include "idt.h"

#include "irq.h"
#include "x86.h"
#include "mem/gdt.h"
#include "common/mem.h"
#include "common/types.h"
#include "descriptors.h"

// --- DEFINE ---

#define INTERRUPT_COUNT   256

// Processor exceptions are located in this range of entries in the IVT
#define FIRST_EXCEPTION   0
#define LAST_EXCEPTION    20
#define EXCEPTION_COUNT   (LAST_EXCEPTION-FIRST_EXCEPTION+1)

// Reprograms the PIC to relocate hardware interrupts starting at IVT entry 32:
// IRQ0  -> Interrupt 32
// IRQ1  -> Interrupt 33
// ...
// IRQ15 -> Interrupt 47
#define IRQ_REMAP_OFFSET  32

// Structure of an IDT descriptor. There are 3 types of descriptors:
// task-gates, interrupt-gates, trap-gates.
// See 5.11 of Intel 64 & IA32 architectures software developer's manual for more details.
// For task gates, offset must be 0.
typedef struct {

    uint16_t offset15_0;   // only used by trap and interrupt gates
    uint16_t selector;     // segment selector for trap and interrupt gates; TSS segment
                           // selector for task gates
    uint16_t reserved : 8;
    uint16_t type : 5;
    uint16_t dpl : 2;
    uint16_t p : 1;
    uint16_t offset31_16;  // only used by trap and interrupt gates
} __attribute__((packed)) idt_entry_t;

// CPU context used when saving/restoring context from an interrupt
typedef struct {

    uint32_t gs, fs, es, ds;
    uint32_t ebp, edi, esi;
    uint32_t edx, ecx, ebx, eax;
    uint32_t number, error_code;
    uint32_t eip, cs, eflags, esp, ss;
} regs_t;

// Structure describing a pointer to the IDT gate table.
// This format is required by the lidt instruction.
typedef struct {

    uint16_t limit;   // Limit of the table (ie. its size)
    uint32_t base;    // Address of the first entry
} __attribute__((packed)) idt_ptr_t;

// Gates table
static idt_entry_t idt[INTERRUPT_COUNT];
static idt_ptr_t   idt_ptr;

// Loads the IDT specified in argument.
// Defined in idt_asm.s
void idt_load(idt_ptr_t *idt_ptr);

// Low-level ISR for processor exceptions.
// These are defined in idt_asm.s
void _exception3();
void _exception8();

// --- STATIC FUNCTION ---

// Builds and returns an IDT entry.
// selector is the code segment selector to access the ISR
// offset is the address of the ISR (for task gates, offset must be 0)
// type indicates the IDT entry type
// dpl is the privilege level required to call the associated ISR
static idt_entry_t idt_build_entry(uint16_t selector, uint32_t offset, uint8_t type, uint8_t dpl) {

    idt_entry_t entry;

    entry.offset15_0 = offset & 0xffff;
    entry.selector = selector;
    entry.reserved = 0;
    entry.type = type;
    entry.dpl = dpl;
    entry.p = 1;
    entry.offset31_16 = (offset >> 16) & 0xffff;

    return entry;
}

// Low-level ISR for hardware interrupts.
// These are defined in idt_asm.s
void _irq5();

// --- FUNCTIONS ---

// High-level ISR for processor exceptions.
void exception_handler(regs_t *regs) {

    // TODO
}

// High-level ISR for hardware interrupts.
void irq_handler(regs_t *regs) {

    // TODO
}

void idt_init() {

    irq_init();

    // TODO

    // Example of IDT entry for exception 3
    idt[3] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR, _exception3, TYPE_INTERRUPT_GATE, DPL_KERNEL);

    // TODO
}
