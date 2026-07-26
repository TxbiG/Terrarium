#include "service.h"

#include "syscall.h"
#include "../status.h"

int terra_service_start(const char *name) {
    if (!name)
        return TERRA_STATUS_INVAL;

    return (int)terra_syscall(TERRA_SYSCALL_SERVICE_START,
                              (uintptr_t)name,
                              0, 0, 0, 0, 0);
}

int terra_service_stop(const char *name) {
    if (!name)
        return TERRA_STATUS_INVAL;

    return (int)terra_syscall(TERRA_SYSCALL_SERVICE_STOP,
                              (uintptr_t)name,
                              0, 0, 0, 0, 0);
}

int terra_service_status(const char *name, terra_service_info_t *out_info) {
    if (!name || !out_info)
        return TERRA_STATUS_INVAL;

    return (int)terra_syscall(TERRA_SYSCALL_SERVICE_STATUS,
                              (uintptr_t)name,
                              (uintptr_t)out_info,
                              0, 0, 0, 0);
}

int terra_service_list(terra_service_info_t *out_services, uint32_t capacity, uint32_t *out_count) {
    if ((!out_services && capacity != 0) || !out_count)
        return TERRA_STATUS_INVAL;

    return (int)terra_syscall(TERRA_SYSCALL_SERVICE_LIST,
                              (uintptr_t)out_services,
                              (uintptr_t)capacity,
                              (uintptr_t)out_count,
                              0, 0, 0);
}