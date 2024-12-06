#ifndef _PAGING_H_
#define _PAGING_H_

#include <stdint.h>

// --- DEFINE ---

#define PAGE_SIZE  4096         // kernel only uses 4KB pages
#define PAGES_IN_PT 1024        // number of pages in a page table
#define PAGETABLES_IN_PD 1024   // number of page table in a page directory

// Returns the index of the PDE for a given virtual address
#define ADDR_TO_PDE(addr) (((uint32_t)addr) >> 22) 

// Returns the index of the page table entry the virtual addr belongs to
#define ADDR_TO_PTE(addr) ((((uint32_t)addr) >> 12) & 0x3FF)

// Converts a 32-bit address into a page number
#define ADDR_TO_PAGE_NB(addr) (((uint32_t)addr) / PAGE_SIZE)

// Returns the number of pages required to store the given number of bytes
#define PAGE_COUNT(size) ((size + PAGE_SIZE - 1) / PAGE_SIZE)

enum privilege_t {

    PRIVILEGE_KERNEL = 0,
    PRIVILEGE_USER = 1
};

enum access_t {

    ACCESS_READONLY = 0,
    ACCESS_READWRITE = 1
};

/**
 * @brief Structure representing a page directory entry (PDE).
 *
 * A page directory is an array of PDEs, each pointing to a page table. Each PDE is responsible for mapping
 * up to 4MB of memory using 1024 page table entries (PTEs), where each PTE maps a 4KB page.
 */
typedef struct {                  // LSB

    uint32_t present : 1;         // page table currently loaded in memory (used for swapping)
    uint32_t rw : 1;              // specifies read-write privileges
    uint32_t user : 1;            // specifies user or supervisor privilege
    uint32_t write_through : 1;   // enable/disable write-through caching for the associated page table
    uint32_t cache_disable : 1;   // enable/disable caching of the associated page table (useful for MMIO)

    uint32_t accessed : 1;        // page table has been accessed by read or write (set by cpu)
    uint32_t reserved : 1;        // reserved (0)
    uint32_t page_sz : 1;         // page size: 0 = 4KB, 1 = 4MB
    uint32_t gp : 1;              // indicates a global page (ignored)
    uint32_t available : 3;       // unused, freely available for kernel use

	  uint32_t pagetable_frame_number : 20; // frame number of the PT this PDE points to
                                          // to get the PT's physical address, multiply this by page size

} __attribute__((packed)) PDE_t;  // MSB

/**
 * @brief Structure representing a page table entry (PTE).
 *
 * A page table is an array of PTEs, each mapping a 4KB page of memory. The PTE stores the frame number
 * of the physical page and various access and status bits.
 * A page can hold 2^12 bytes = 4096 bytes = 4KB.
 */
typedef struct {                  // LSB

    uint32_t present : 1;         // page currently loaded in memory (used for swapping)
    uint32_t rw : 1;              // specifies read-write privileges
    uint32_t user : 1;            // specifies user or supervisor privilege
    uint32_t write_through : 1;   // enable/disable write-through caching for the associated page
    uint32_t cache_disable : 1;   // enable/disable caching of the associated page (useful for MMIO)

    uint32_t accessed : 1;        // page has been accessed by read or write (set by cpu)
    uint32_t dirty : 1;           // page has been written to (set by cpu)
    uint32_t pat : 1;             // only used when using "Page Attribute Table" (PAT), 0 otherwise
    uint32_t gp : 1;              // indicates a global page
    uint32_t available : 3;       // unused, freely available for kernel use

    uint32_t frame_number : 20;   // frame number the page points to

} __attribute__((packed)) PTE_t;  // MSB

// --- FUNCTION ---

/**
 * @brief Initializes paging for the kernel.
 *
 * @param RAM_in_KB  Total available RAM in kilobytes.
 *
 * This function sets up the kernel's page directory with identity mappings for the available RAM
 * and the VBE framebuffer, then loads the page directory and enables paging.
 * This function start by initialize the frame.
 */
void paging_init(uint32_t RAM_in_KB);

/**
 * @brief Maps virtual to physical memory in the specified page directory.
 *
 * @param pagedir     Pointer to the page directory.
 * @param virt_addr   Starting virtual address to map.
 * @param phys_addr   Starting physical address to map.
 * @param size        Size of the memory region to map.
 * @param privilege   Privilege level (kernel or user).
 * @param access      Access type (read-only or read-write).
 *
 * This function maps a contiguous virtual memory region to a contiguous physical memory region
 * using 4KB pages. It dynamically allocates frames for the page tables as needed.
 * Both `virt_addr` and `phys_addr` must be page-aligned (4KB).
 */
void paging_mmap(PDE_t *pagedir, uint32_t virt_addr, uint32_t phys_addr, uint32_t size, enum privilege_t privilege, enum access_t access);

/**
 * @brief Enables paging on the CPU.
 *
 * This is an assembly function that activates paging by setting the appropriate control register.
 */
void paging_enable();

/**
 * @brief Loads the specified page directory into the CPU.
 *
 * @param pagedir  Pointer to the page directory to load.
 *
 * This is an assembly function that sets the active page directory in the CPU's control register.
 */
void paging_load_pagedir(PDE_t *pagedir);

/**
 * @brief Returns the current page directory being used by the CPU.
 *
 * @return Pointer to the current page directory.
 *
 * This is an assembly function that retrieves the page directory currently in use by the CPU.
 */
PDE_t *paging_get_current_pagedir();

// Allocates size bytes of physical memory (as frames, so not necessarily contiguous), then maps these
// frames at virtual address virt_addr. The area at virt_addr is guaranteed to be contiguous.
// The mapping is performed in the specified page directory.
// This function dynamically allocates the necessary frames to store the page tables and stores
// their addresses into page_tables so that they can be subsequently freed.
// If NULL is passed in page_tables, then the allocated frames' addresses won't be stored.
// IMPORTANT: virt_addr and phys_addr must be aligned to a page size (4KB).
// Returns the number of frames allocated (including frames used to store page tables).
uint32_t paging_alloc(PDE_t *pagedir, PTE_t *page_tables[PAGETABLES_IN_PD], uint32_t virt_addr, uint32_t size, enum privilege_t privilege);

#endif // _PAGING_H_
