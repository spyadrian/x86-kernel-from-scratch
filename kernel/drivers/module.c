#include "module.h"

#include <stdint.h>
#include "boot/multiboot.h"
#include "boot/multiboot_structs.h"
#include "common/string.h"

// --- DEFINE ---

// ...

// --- STATIC ---

static multiboot_info_t *_mbi;
static multiboot_module_t *_mods;

// --- FUNCTION ---

void module_init() {

    _mbi = multiboot_get_info();
    _mods = (multiboot_module_t *)(_mbi->mods_addr);
}

uint32_t module_count() {

    return (uint32_t)_mbi->mods_count;
}

void *module_get_addr(uint16_t idx) {

    return (void *)_mods[idx].mod_start;
}

uint32_t module_size(uint16_t idx) {

    return (uint32_t)(_mods[idx].mod_start - _mods[idx].mod_end);
}

char *module_cmdline(uint16_t idx) {

    return (char *)_mods[idx].cmdline;
}

void module_get_arg(uint16_t idx, uint8_t nb_arg, char *arg) {

    char *cmd = module_cmdline(idx);
    uint8_t nb_char = 0;
    char *start;

    while (nb_arg != 0) {
        if (*cmd == ' ' || *cmd == 0) nb_arg--;
        cmd++;
    }

    start = cmd;

    while (*cmd != 0 && *cmd != ' ') {
        nb_char++;
        cmd++;
    }

    strncpy(arg, start, nb_char);
}

void *module_last_addr() {

    return (void *)_mods[module_count() - 1].mod_end;
}

