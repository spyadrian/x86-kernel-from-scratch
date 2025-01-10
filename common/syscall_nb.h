#ifndef _SYSCALL_NB_COMMON_H_
#define _SYSCALL_NB_COMMON_H_

typedef enum {
    SYSCALL_KEY = 1,
    SYSCALL_TIMER_INFO = 2,
    SYSCALL_TIMER_SLEEP = 3,
    SYSCALL_SETPIXEL = 4,
    SYSCALL_TASK_EXEC = 5,

    __SYSCALL_END__
} syscall_t;

#endif
