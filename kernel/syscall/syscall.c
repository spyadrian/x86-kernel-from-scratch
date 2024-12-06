#include "common/types.h"
#include "syscall.h"

// Dummy syscall
static int syscall_dummy(uint32_t arg1, uint32_t arg2, uint32_t arg3, uint32_t arg4) {
    UNUSED(arg1);
    UNUSED(arg2);
    UNUSED(arg3);
    UNUSED(arg4);
    return 0;
}

// Called by the assembly function: _syscall_handler
int syscall_handler(syscall_t nb, uint32_t arg1, uint32_t arg2, uint32_t arg3, uint32_t arg4) {
    // Call the syscall function based on nb.
    // Make sure to return an error in case of an invalid syscall number.
}
