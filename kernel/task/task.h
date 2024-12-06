#ifndef _TASK_H_
#define _TASK_H_

#include "common/types.h"
#include "mem/paging.h"
#include "tss.h"

#define MAX_TASK_COUNT  8

// Virtual address (1GB) where task user code/data is mapped (i.e. application entry point)
#define TASK_VIRT_ADDR 0x40000000

// Structure of a task.
typedef struct {
    PDE_t pagedir[PAGETABLES_IN_PD] __attribute__((aligned(4096)));  // Task page directory
    tss_t tss __attribute__((aligned(4096)));   // Context of the task (must not cross a page boundary!)
    PTE_t *page_tables[PAGES_IN_PT];            // Save pointers to page tables in order to deallocate
                                                // previously allocated frames at task termination
    uint16_t tss_selector;                      // selector required when switching to the task
    uint8_t kernel_stack[65536];                // kernel stack
    uint32_t virt_addr;                         // Start of the task's virtual address space
    uint32_t addr_space_size;                   // Size of the task's address space in bytes
    // TODO: add whathever you'd like...
} task_t;

void tasks_init();

task_t *task_get_current();

bool task_exec(char *filename);

// Implemented in task_asm.s
void task_ltr(uint16_t tss_selector);
void task_switch(uint16_t tss_selector);
uint16_t task_get_current_sel();

#endif
