#include "process.h"

#include "syscall.h"
#include "../status.h"

terra_pid_t terra_process_spawn(const char *path, char *const argv[]) {
    if (!path)
        return (terra_pid_t)TERRA_STATUS_INVAL;

    return (terra_pid_t)terra_syscall(TERRA_SYSCALL_PROC_SPAWN,
                                      (uintptr_t)path,
                                      (uintptr_t)argv,
                                      0, 0, 0, 0);
}

int terra_process_wait(terra_pid_t pid, int *out_status) {
    return (int)terra_syscall(TERRA_SYSCALL_PROC_WAIT,
                              (uintptr_t)pid,
                              (uintptr_t)out_status,
                              0, 0, 0, 0);
}

int terra_process_kill(terra_pid_t pid, int signal) {
    return (int)terra_syscall(TERRA_SYSCALL_PROC_KILL,
                              (uintptr_t)pid,
                              (uintptr_t)signal,
                              0, 0, 0, 0);
}

int terra_process_info(terra_pid_t pid, terra_process_info_t *out_info) {
    if (!out_info)
        return TERRA_STATUS_INVAL;

    return (int)terra_syscall(TERRA_SYSCALL_PROC_INFO,
                              (uintptr_t)pid,
                              (uintptr_t)out_info,
                              0, 0, 0, 0);
}