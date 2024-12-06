#include "task.h"

#define TASK_STACK_SIZE_MB 2

static task_t tasks[MAX_TASK_COUNT];
static tss_t initial_tss;
static uint8_t initial_tss_kernel_stack[65536];

// Initializes the task subsystem.
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
}

// Returns a pointer to the current task.
// If NULL is returned it means we are in the kernel.
task_t *task_get_current() {
    return NULL;
}

// Creates and returns a task from the fixed pool of tasks.
// This function dynamically allocates the address space of size "size" (in bytes) for the task.
// Returns NULL if failed.
static task_t *task_create(char *name, uint_t addr_space_size) {
}

// Frees a task previously created with task_create().
// This function frees the task's page frames.
static void task_free(task_t *t) {
}

// Creates a new task with the content of the specified binary application.
// Once loaded, the task is ready to be executed.
// Returns NULL if failed (ie. reached max number of tasks).
static task_t *task_load(char *filename) {
}

// Loads a task and executes it.
// Returns false if failed.
bool task_exec(char *filename) {
}
