#ifndef TERRARIUM_SYSTEM_FS_QUOTA_H
#define TERRARIUM_SYSTEM_FS_QUOTA_H

#include <stdint.h>

typedef struct terra_quota {
    uint32_t id;
    uint64_t soft_bytes;
    uint64_t hard_bytes;
    uint64_t used_bytes;
} terra_quota_t;

int terra_quota_get(const char *mountpoint, uint32_t id, terra_quota_t *out_quota);
int terra_quota_set(const char *mountpoint, const terra_quota_t *quota);

#endif
