#include "syscall.h"

#include "../status.h"

long terra_syscall(uint32_t number, uintptr_t a0, uintptr_t a1, uintptr_t a2,
                   uintptr_t a3, uintptr_t a4, uintptr_t a5) {
    (void)number;
    (void)a0;
    (void)a1;
    (void)a2;
    (void)a3;
    (void)a4;
    (void)a5;

    return TERRA_STATUS_NOSYS;
}