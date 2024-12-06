#include "paging.h"

#include <stdint.h>
#include "boot/multiboot.h"
#include "boot/multiboot_structs.h"
#include "frame.h"
#include "x86.h"

// --- DEFINE ---

extern uint32_t ld_stack_top;   // defined in kernel.ld
#define STACK_TOP ((uint32_t)&ld_stack_top)

// framebuffer's virtual address mapped at 3GB
#define VIRTUAL_FRAMEBUFFER_ADDR 0xC0000000

static PDE_t kernel_pde[PAGETABLES_IN_PD] __attribute__((aligned(PAGE_SIZE)));

// --- STATIC ---

/**
 * @brief Maps a single page into the specified page directory.
 *
 * @param pde        Pointer to the page directory.
 * @param virt_addr  Virtual address of the page to map.
 * @param phys_addr  Physical address (frame) of the page.
 * @param privilege  Privilege level (kernel or user) for the mapping.
 * @param access     Access type (read/write) for the page.
 *
 * This function maps a virtual address to a physical address by creating or modifying the
 * appropriate page directory entry (PDE) and page table entry (PTE). If the PDE is not
 * present (i.e., the page table does not exist), it dynamically allocates a new frame for the
 * page table and updates the PDE. Then, it writes the PTE for the given virtual address.
 * Both `virt_addr` and `phys_addr` must be page-aligned.
 *
 * @return A pointer to the newly allocated page table (if a new one was created), or 0 if the
 * page table already existed.
 */
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

/**
 * @brief Unmaps a single page from the specified page directory.
 *
 * @param pde       Pointer to the page directory.
 * @param virt_addr Virtual address of the page to unmap.
 *
 * This function unmaps the virtual address by clearing the corresponding page table entry (PTE).
 * If the page directory entry (PDE) is not present, it halts, as this indicates an invalid 
 * mapping.
 */
static void unmap_page(PDE_t *pde, uint32_t virt_addr) {

    uint32_t pde_idx = ADDR_TO_PDE(virt_addr);
    uint32_t pte_idx = ADDR_TO_PTE(virt_addr);

    if (!pde[pde_idx].present) halt();

    uint32_t pt_phys = FRAME_NB_TO_ADDR(pde[pde_idx].pagetable_frame_number);
    PTE_t *pte = (PTE_t *)pt_phys;

    // free frame use by the page
    frame_free((void *)FRAME_NB_TO_ADDR(pte->frame_number));
    pte[pte_idx] = (PTE_t){0};

    // free page table frame number if not use anymore
    for (uint32_t i = 0; i < PAGES_IN_PT; i++) {

        if (pte[i].present == 1)
            return;
    }

    frame_free((void *)FRAME_NB_TO_ADDR(pde[pde_idx].pagetable_frame_number));
    pde[pde_idx] = (PDE_t){0};
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

uint32_t paging_alloc(PDE_t *pagedir, PTE_t *page_tables[PAGETABLES_IN_PD], uint32_t virt_addr, uint32_t size, enum privilege_t privilege) {

    // virtual address must be aligned to PAGE_SIZE!
    if (virt_addr % PAGE_SIZE) halt();

    // skip entries of already allocated pagetables since we'll need to store newly allocated page tables
    uint_t pt_offset = 0;
    while (page_tables && page_tables[pt_offset])
        pt_offset++;

    int pt_count = 0;
    uint_t frame_count = PAGE_COUNT(size); // how many frames do we need?
    if (frame_count == 0) return 0;

    for (uint_t i = 0; i < frame_count; i++) {

        uint32_t frame_addr = (uint32_t)frame_alloc();
        PTE_t *page_table = mmap_page(pagedir, virt_addr, frame_addr, privilege, ACCESS_READWRITE);

        // A new page table was just allocated, store it into the input page_tables array of pointer to page tables
        if (page_tables && page_table)
            page_tables[pt_offset + pt_count++] = page_table;

        virt_addr += PAGE_SIZE;
    }

    return pt_count+frame_count;
}
