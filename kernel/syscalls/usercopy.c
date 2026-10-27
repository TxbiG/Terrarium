#include "usercopy.h"
#include "../../system/status.h"
#include <stdint.h>

/* Fail closed until a real per-process VM-aware copyin/copyout is connected. */
__attribute__((weak)) int terra_copy_from_user(void *kernel_dst,
                                               uintptr_t user_src,
                                               size_t size)
{
    (void)kernel_dst;
    (void)user_src;
    (void)size;
    return TERRA_STATUS_NOSYS;
}

__attribute__((weak)) int terra_copy_to_user(uintptr_t user_dst,
                                             const void *kernel_src,
                                             size_t size)
{
    (void)user_dst;
    (void)kernel_src;
    (void)size;
    return TERRA_STATUS_NOSYS;
}

int terra_copy_from_user_string(char *kernel_dst, size_t capacity,
                                uintptr_t user_src)
{
    size_t i;
    if (!kernel_dst || capacity == 0 || user_src == 0)
        return TERRA_STATUS_INVAL;
    kernel_dst[0] = '\0';
    for (i = 0; i < capacity; ++i) {
        int status;
        if ((uintptr_t)i > UINTPTR_MAX - user_src) {
            kernel_dst[0] = '\0';
            return TERRA_STATUS_INVAL;
        }
        status = terra_copy_from_user(&kernel_dst[i], user_src + (uintptr_t)i, 1);
        if (status != TERRA_STATUS_OK) {
            kernel_dst[0] = '\0';
            return status;
        }
        if (kernel_dst[i] == '\0')
            return TERRA_STATUS_OK;
    }
    kernel_dst[0] = '\0';
    return TERRA_STATUS_INVAL; /* no NUL terminator within the bounded string */
}
