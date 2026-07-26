#include "disk_util.h"
#include "../status.h"
#include <stdio.h>

int terra_disk_human_size(uint64_t bytes, char *out_text, unsigned long out_size) {
    if (!out_text || out_size == 0)
        return TERRA_STATUS_INVAL;
    if (bytes >= 1024ull * 1024ull * 1024ull)
        snprintf(out_text, out_size, "%llu GiB", (unsigned long long)(bytes / (1024ull * 1024ull * 1024ull)));
    else if (bytes >= 1024ull * 1024ull)
        snprintf(out_text, out_size, "%llu MiB", (unsigned long long)(bytes / (1024ull * 1024ull)));
    else if (bytes >= 1024ull)
        snprintf(out_text, out_size, "%llu KiB", (unsigned long long)(bytes / 1024ull));
    else
        snprintf(out_text, out_size, "%llu B", (unsigned long long)bytes);
    return TERRA_STATUS_OK;
}

int terra_disk_usage(const char *path, uint64_t *out_used, uint64_t *out_total) {
    (void)path;
    (void)out_used;
    (void)out_total;
    return TERRA_STATUS_NOSYS;
}
