#include "idt.h"

#include <stdint.h>

#include "common/colors.h"
#include "drivers/gfx.h"
#include "irq.h"
#include "x86.h"
#include "mem/gdt.h"
#include "common/mem.h"
#include "descriptors.h"
#include "drivers/pic.h"
#include "syscall/syscall.h"

// --- DEFINE ---

#define INTERRUPT_COUNT   256

#define FIRST_EXCEPTION   0
#define LAST_EXCEPTION    20
#define EXCEPTION_COUNT   (LAST_EXCEPTION - FIRST_EXCEPTION + 1)

// Reprograms the PIC to relocate hardware interrupts starting at IVT entry 32:
// IRQ0  -> Interrupt 32, ..., IRQ15 -> Interrupt 47
#define IRQ_REMAP_OFFSET  32
#define FIRST_IRQ         0
#define LAST_IRQ          14

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

static const char *EXCEPTION_NAMES[] = {

    "Division by zero",
    "Debug",
    "Non-maskable Interrupt",
    "Breakpoint",
    "Overflow",
    "Bound Range Exceeded",
    "Invalid Opcode",
    "Device Not Available",
    "Double Fault",
    "Coprocessor Segment Overrun",
    "Invalid TSS",
    "Segment Not Present",
    "Stack-Segment Fault",
    "General Protection Fault",
    "Page Fault",
    "Reserved",
    "x87 Floating-Point Exception",
    "Alignment Check",
    "Machine Check",
    "SIMD Floating-Point Exception",
    "Virtualization Exception"
};

/*
static const char *IRQ_NAMES[] = {

    "System timer (PIT)",
    "Keyboard",
    "Redirected to slave PIC",
    "Serial port (COM2/COM4)",
    "Serial port (COM1/COM3)",
    "Sound card",
    "Floppy disk controller",
    "Parallel port",
    "Real time clock",
    "Redirected to IRQ2",
    "Reserved",
    "Reserved",
    "P/S mouse",
    "Math coprocessor",
    "Hard disk controller",
    "Reserved"
};
*/

// --- ASSEMBLY FUNCTION ---

// Loads the IDT specified in argument, defined in idt_asm.s
void idt_load(idt_ptr_t *idt_ptr);

// Low-level ISR for processor exceptions, these are defined in idt_asm.s
void _exception0();  void _exception1();  void _exception2();
void _exception3();  void _exception4();  void _exception5();
void _exception6();  void _exception7();  void _exception8();
void _exception9();  void _exception10(); void _exception11();
void _exception12(); void _exception13(); void _exception14();
void _exception15(); void _exception16(); void _exception17();
void _exception18(); void _exception19(); void _exception20();

// Low-level ISR for processor interrupts, these are defined in idt_asm.s
void _irq0();  void _irq1();  void _irq2();  void _irq3();
void _irq4();  void _irq5();  void _irq6();  void _irq7();
void _irq8();  void _irq9();  void _irq10(); void _irq11();
void _irq12(); void _irq13(); void _irq14(); void _irq15();

// Create an array of function pointers
void (*_EXCEPTIONS_IRQS[])() = {

    // exception
    _exception0,  _exception1,  _exception2,
    _exception3,  _exception4,  _exception5,
    _exception6,  _exception7,  _exception8,
    _exception9,  _exception10, _exception11,
    _exception12, _exception13, _exception14,
    _exception15, _exception16, _exception17,
    _exception18, _exception19, _exception20,

    // interrupts
    _irq0,  _irq1,  _irq2,  _irq3,
    _irq4,  _irq5,  _irq6,  _irq7,
    _irq8,  _irq9,  _irq10, _irq11,
    _irq12, _irq13, _irq14,
};

// --- STATIC FUNCTION ---

/**
 * @brief Builds an IDT entry with the specified parameters.
 *
 * Constructs an IDT entry with the given code segment selector, ISR offset,
 * entry type, and privilege level. This entry is used to define the behavior 
 * of interrupts and exceptions.
 *
 * @param selector Code segment selector for accessing the ISR.
 * @param offset   Address of the Interrupt Service Routine (ISR).
 *                 For task gates, this value must be 0.
 * @param type     Type of the IDT entry (e.g., interrupt gate).
 * @param dpl      Descriptor Privilege Level required to invoke the ISR.
 * @return A fully populated IDT entry.
 */
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

/**
 * @brief Handles hardware interrupts.
 *
 * This function is invoked for all hardware-generated interrupts.
 * It identifies and executes the associated interrupt handler and
 * sends an End-of-Interrupt (EOI) signal to the Programmable Interrupt
 * Controller (PIC).
 *
 * @param regs Pointer to the register state at the time of the interrupt.
 */
void exception_handler(regs_t *regs) {

    gfx_printf_colored(RED, BLACK, "EXCEPTION : %s\n", EXCEPTION_NAMES[regs->number]);
    halt();
}

/**
 * @brief Handles hardware interrupts.
 *
 * This function is invoked for all hardware-generated interrupts.
 * It identifies and executes the associated interrupt handler and
 * sends an End-of-Interrupt (EOI) signal to the Programmable Interrupt
 * Controller (PIC).
 *
 * @param regs Pointer to the register state at the time of the interrupt.
 */
void irq_handler(regs_t *regs) {

    uint32_t irq = regs->number;

    // gfx_printf_colored(YELLOW, BLACK, "INTERRUPT : %s\n", IRQ_NAMES[regs->number]);
    handler_t *hd = irq_get_handler(irq);
    if (hd != NULL) hd->func();

    pic_eoi(irq);
}

// --- FUNCTIONS ---

void idt_init() {

    pic_init();
    irq_init();

    // initialize an IDT table full of zero
    memset(idt, 0, sizeof(idt));
    idt_ptr.limit = sizeof(idt) - 1;
    idt_ptr.base = (uint32_t)&idt;

    uint32_t j = 0;
    // build entry for exceptions
    for (uint32_t i = FIRST_EXCEPTION; i <= LAST_EXCEPTION; i++)
        idt[i] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR, (uint32_t)_EXCEPTIONS_IRQS[j++], TYPE_INTERRUPT_GATE, DPL_KERNEL);

    // build entry for interrupts
    for (uint32_t i = FIRST_IRQ; i <= LAST_IRQ; i++)
        idt[i + IRQ_REMAP_OFFSET] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR, (uint32_t)_EXCEPTIONS_IRQS[j++], TYPE_INTERRUPT_GATE, DPL_KERNEL);

    idt[48] = idt_build_entry(GDT_KERNEL_CODE_SELECTOR, (uint32_t)_syscall_handler, TYPE_TRAP_GATE, DPL_USER);

    // load the idt on the CPU
    idt_load(&idt_ptr);
}
