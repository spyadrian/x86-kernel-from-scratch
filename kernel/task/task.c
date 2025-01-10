#include "task.h"

#include <stdint.h>
#include <stdbool.h>

#include "x86.h"
#include "tss.h"
#include "mem/gdt.h"
#include "mem/frame.h"
#include "mem/paging.h"
#include "common/mem.h"
#include "descriptors.h"
#include "common/string.h"
#include "boot/multiboot.h"
#include "drivers/module.h"

#include "drivers/gfx.h"

// --- DEFINE ---

#define TASK_STACK_SIZE_MB 2

static task_t tasks[MAX_TASK_COUNT];
static tss_t initial_tss;
static uint8_t initial_tss_kernel_stack[65536];
static uint8_t current_task_idx;

// --- STATIC FUNCTION ---

// Creates and returns a task from the fixed pool of tasks.
// This function dynamically allocates the address space of size "size" (in bytes) for the task.
// Returns NULL if failed.
static task_t *task_create(uint32_t addr_space_size) {

    task_t *t = tasks + current_task_idx;
    memset(t, 0, sizeof(task_t));

    t->virt_addr = TASK_VIRT_ADDR;
    t->addr_space_size = addr_space_size;

    // adds the task's TSS to the GDT and store its selector
    extern gdt_entry_t *gdt_first_free_entry;
    gdt_entry_t *gdt_task_tss = gdt_first_free_entry + current_task_idx;
    int32_t gdt_tss_sel = gdt_entry_to_selector(gdt_task_tss);
    *gdt_task_tss = gdt_make_tss(&t->tss, DPL_KERNEL);
    t->tss_selector = gdt_tss_sel;

    // sets up the task's initial content
    tss_t *tss = &t->tss;
    tss->cs = GDT_USER_CODE_SELECTOR;                           // user code segment
    tss->ds = tss->es = tss->fs = tss->gs = tss->ss = GDT_USER_DATA_SELECTOR;
    tss->cr3 = (uint32_t)t->pagedir;                            // task's page directory
    tss->eflags = (1 << 9);                                     // enable hardware interrupts
    tss->ss0 = GDT_KERNEL_DATA_SELECTOR;                        // task's kernel stack segment
    tss->esp0 = (uint32_t)(t->kernel_stack) + sizeof(t->kernel_stack);      // task's kernel stack ptr
    tss->eip = TASK_VIRT_ADDR;                                  // task's instruction ptr
    tss->esp = tss->ebp = TASK_VIRT_ADDR + t->addr_space_size + TASK_STACK_SIZE_MB * 1024 * 1024;  // task's stack and stack frame ptr

    // identity maps available RAM so kernel code can access its own code/data
    paging_mmap(t->pagedir, 0, 0, multiboot_get_RAM_in_KB() * 1024, PRIVILEGE_KERNEL, ACCESS_READWRITE);

    // identity maps framebuffer so task code can acces it
    multiboot_info_t *mbi = multiboot_get_info();
    uint32_t framebuffer_size = mbi->framebuffer_pitch * mbi->framebuffer_height;
    paging_mmap(t->pagedir, mbi->framebuffer_addr, mbi->framebuffer_addr, framebuffer_size, PRIVILEGE_USER, ACCESS_READWRITE);

    // page and frame alloc for task code, data and stack
    paging_alloc(t->pagedir, t->page_tables, t->virt_addr, t->addr_space_size + TASK_STACK_SIZE_MB * 1024 * 1024, PRIVILEGE_USER);

    current_task_idx += 1;
    return t;
}

// Frees a task previously created with task_create().
// This function frees the task's page frames.
static void task_free(task_t *t) {

    for (uint16_t i = 0; i < PAGES_IN_PT; i++) {
        if (t->page_tables[i])
            frame_free((void *)FRAME_NB_TO_ADDR(t->page_tables[i]->frame_number));
    }

    frame_free((void *)t->page_tables);

    current_task_idx -= 1;
}

// Creates a new task with the content of the specified binary application.
// Once loaded, the task is ready to be executed.
// Returns NULL if failed (ie. reached max number of tasks).
static task_t *task_load(char *name) {

    char n[64] = "";
    if (current_task_idx == MAX_TASK_COUNT || strcmp(name, n) == 0)
        return NULL;

    void *task_code = NULL;
    uint32_t task_size = 0;
    for (uint8_t i = 0; i < module_count(); i++) {

        module_get_arg(i, 0, n);

        if (strcmp(name, n) == 0) {

            task_code = module_get_addr(i);
            task_size = module_size(i);
            break;
        }
    }

    if (task_code == NULL)
        return NULL;

    task_t *t = task_create(task_size);

    PDE_t *pde_backup = paging_get_current_pagedir();

    paging_load_pagedir(t->pagedir);

    memcpy((void *)t->virt_addr, task_code, t->addr_space_size);

    paging_load_pagedir(pde_backup);

    return t;
}

// --- FUNCTION ---

void tasks_init() {

    memset(tasks, 0, sizeof(tasks));

    // Allocates and initializes the initial TSS: it is where the CPU state
    // will be saved before switching from the kernel to the very first user task.
    extern gdt_entry_t *gdt_initial_tss;
    *gdt_initial_tss = gdt_make_tss(&initial_tss, DPL_KERNEL);

    memset(&initial_tss, 0, sizeof(tss_t));
    initial_tss.ss0 = GDT_KERNEL_DATA_SELECTOR;
    initial_tss.esp0 = ((uint32_t)initial_tss_kernel_stack) + sizeof(initial_tss_kernel_stack);
    initial_tss.cr3 = (uint32_t)paging_get_current_pagedir();

    // Loads the task register to point to the initial TSS selector.
    // IMPORTANT: The GDT must already be loaded before loading the task register!
    task_ltr(gdt_entry_to_selector(gdt_initial_tss));

    // set the current task
    current_task_idx = 0;
}

task_t *task_get_current() {

    if (current_task_idx == 0) return NULL;

    return &tasks[current_task_idx];
}

bool task_exec(char *filename) {

    task_t *t = task_load(filename);
    if (t == NULL) return false;

    task_switch(t->tss_selector);

    task_free(t);
    return true;
}
