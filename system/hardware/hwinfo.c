#include "hwinfo.h"

#include "../runtime/syscall.h"
#include "../status.h"

int terra_hwinfo_query(terra_hwinfo_t *out_info) {
    if (!out_info)
        return TERRA_STATUS_INVAL;

    return (int)terra_syscall(TERRA_SYSCALL_HWINFO,
                              (uintptr_t)out_info,
                              0, 0, 0, 0, 0);
}