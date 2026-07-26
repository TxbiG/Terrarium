#ifndef TERRARIUM_SYSCALLS_COMPAT_H
#define TERRARIUM_SYSCALLS_COMPAT_H

#include "syscall_table.h"

static inline void registerSyscall(uint32_t id, void *handler) {
    (void)syscalls_register(id, (terra_syscall_fn_t)handler);
}

#endif
