#ifndef TERRARIUM_SYSTEM_HARDWARE_HWINFO_H
#define TERRARIUM_SYSTEM_HARDWARE_HWINFO_H

#include <stdint.h>

typedef struct terra_hwinfo {
    char machine[64];
    char firmware[64];
    uint64_t memory_bytes;
    uint32_t cpu_count;
} terra_hwinfo_t;

int terra_hwinfo_query(terra_hwinfo_t *out_info);

#endif
