#include "paging.h"

#include <stdint.h>
#include "boot/multiboot.h"
#include "boot/multiboot_structs.h"
#include "frame.h"
#include "common/mem.h"
#include "x86.h"

// --- DEFINE ---

#define CHECK_PAGE_ALIGN(addr) ()

extern uint32_t ld_stack_top;   // defined in kernel.ld
#define STACK_TOP ((uint32_t)&ld_stack_top)

// framebuffer's virtual address mapped at 3GB
#define VIRTUAL_FRAMEBUFFER_ADDR 0xC0000000

static PDE_t kernel_pde[PAGETABLES_IN_PD] __attribute__((aligned(PAGE_SIZE)));

// --- STATIC ---

// Map, in the specified page directory, the page starting at virt_addr into
// the physical address (frame) phys_addr.
// This function dynamically allocates the necessary memory (frames) to store the
// page table (if not previously allocated).
// Return the new allocated page table if one was just allocated, otherwise returns 0.
static PTE_t *mmap_page(PDE_t *pde, uint32_t virt_addr, uint32_t phys_addr, enum privilege_t privilege, enum access_t access) {

    // check for alignment
    if (virt_addr % PAGE_SIZE != 0 || phys_addr % PAGE_SIZE != 0) halt();

    uint32_t pde_idx = ADDR_TO_PDE(virt_addr);
    uint32_t pte_idx = ADDR_TO_PTE(virt_addr);

    if (!pde[pde_idx].present) {

        // - write pde -
        void *frame_addr = frame_alloc();
        if (frame_addr == (void *)0xFFFFFFFF) halt();

        pde[pde_idx].present = 1;
        pde[pde_idx].rw = access;
        pde[pde_idx].user = privilege;
        pde[pde_idx].page_sz = 0;
        pde[pde_idx].pagetable_frame_number = ADDR_TO_FRAME_NB(frame_addr);

        // - write pte -
        PTE_t *pte = (PTE_t *)frame_addr;
        pte[pte_idx].present = 1;
        pte[pte_idx].rw = access;
        pte[pte_idx].user = privilege;
        pte[pte_idx].frame_number = ADDR_TO_FRAME_NB(phys_addr);

        return pte;
    }

    // get address of the already existing table
    uint32_t pt_phys = FRAME_NB_TO_ADDR(pde[pde_idx].pagetable_frame_number);
    PTE_t *pte = (PTE_t *)pt_phys;

    // - write pte -
    pte[pte_idx].present = 1;
    pte[pte_idx].rw = access;
    pte[pte_idx].user = privilege;
    pte[pte_idx].frame_number = ADDR_TO_FRAME_NB(phys_addr);

    return 0;
}

static void unmap_page(PDE_t *pde, uint32_t virt_addr) {

     uint32_t pde_idx = ADDR_TO_PDE(virt_addr);
     uint32_t pte_idx = ADDR_TO_PTE(virt_addr);

     if (!pde[pde_idx].present) halt();

    uint32_t pt_phys = FRAME_NB_TO_ADDR(pde[pde_idx].pagetable_frame_number);
    PTE_t *pte = (PTE_t *)pt_phys;

    pte[pte_idx] = (PTE_t){0};
}

// --- FUNCTION ---

void paging_init(uint32_t RAM_in_KB) {

    multiboot_info_t *mbi = multiboot_get_info();

    // - init frame -
    frame_init(RAM_in_KB);

    // - identity mapping RAM -
    paging_mmap(kernel_pde, 0x0, 0x0, RAM_in_KB * 1024, PRIVILEGE_KERNEL, ACCESS_READWRITE);

    // - identity mapping framebuffer -
    uint32_t framebuffer_size = mbi->framebuffer_pitch * mbi->framebuffer_height;

    paging_mmap(kernel_pde, mbi->framebuffer_addr, mbi->framebuffer_addr, framebuffer_size, PRIVILEGE_KERNEL, ACCESS_READWRITE);
    paging_mmap(kernel_pde, VIRTUAL_FRAMEBUFFER_ADDR, mbi->framebuffer_addr, framebuffer_size, PRIVILEGE_KERNEL, ACCESS_READWRITE);

    // - mapping guard page -
    unmap_page(kernel_pde, 0x0);
    unmap_page(kernel_pde, STACK_TOP);

    // - load page directory and enable paging -
    paging_load_pagedir(kernel_pde);
    paging_enable();
}

void paging_mmap(PDE_t *pde, uint32_t virt_addr, uint32_t phys_addr, uint32_t size, enum privilege_t privilege, enum access_t access) {

    // check for alignment
    if (virt_addr % PAGE_SIZE != 0 || phys_addr % PAGE_SIZE != 0) halt();

    uint32_t nb_page = PAGE_COUNT(size);

    for (uint32_t i = 0; i < nb_page; i++) {

        mmap_page(pde, virt_addr + (i * PAGE_SIZE), phys_addr + (i * PAGE_SIZE), privilege, access);
    }
}
