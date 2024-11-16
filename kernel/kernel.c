#include <stdint.h>

#include "x86.h"
#include "mem/gdt.h"
#include "mem/paging.h"
#include "drivers/gfx.h"
#include "common/string.h"
#include "drivers/timer.h"
#include "interrupt/idt.h"
#include "drivers/module.h"
#include "boot/multiboot.h"

// -- DEFINE --

// These are defined in the linker script: kernel.ld
extern void ld_kernel_start();
extern void ld_kernel_end();
#define KERNEL_START  ((uint32_t)&ld_kernel_start)
#define KERNEL_END    ((uint32_t)&ld_kernel_end)

// -- FUNCTION --

void render_start_screen() {

    multiboot_info_t *mbi = multiboot_get_info();
    uint32_t mcount = module_count();

    // - general info -
    gfx_printf("YoctOS started\n\n");
    gfx_printf("VBE mode %dx%d %dbpp initialized (addr=0x%x, pitch=%d).\n",
               mbi->framebuffer_width, mbi->framebuffer_height, mbi->framebuffer_bpp, (uint32_t)mbi->framebuffer_addr, mbi->framebuffer_pitch);
    gfx_printf("Detected %d of RAM\n", multiboot_get_RAM_in_KB());
    gfx_printf("Kernel loaded at [0x%x-0x%x], size=%dKB\n",
               KERNEL_START, KERNEL_END, (KERNEL_END - KERNEL_START) / 1024);

    // - modules info -
    gfx_printf("%d module(s) found.\n", mcount);

    for (uint32_t i = 0; i < mcount; i++) {

        gfx_printf("Module %d: addr=0x%x, size=%d bytes, cmdline=\"%s\"\n",
                   i, module_get_addr(i), module_size(i), module_cmdline(i));
    }

    // - welcome message -
    char *msg = module_get_addr(0);
    gfx_printf("%s\n", msg);

    // - logo image -
    gfx_render_image_by_index(1, gfx_get_cursor_x() + 10, gfx_get_cursor_y());
}

// -- MAIN --

void kernel_main(multiboot_info_t *mbi) {

    multiboot_set_info(mbi);

    gdt_init();
    module_init();
    paging_init(multiboot_get_RAM_in_KB());
    gfx_init();
    idt_init();
    timer_init(30);

    sti();

    render_start_screen();
}
