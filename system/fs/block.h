#ifndef TERRARIUM_SYSTEM_FS_BLOCK_H
#define TERRARIUM_SYSTEM_FS_BLOCK_H

#include <stdint.h>

typedef struct terra_block_device {
    char name[64];
    uint64_t bytes;
    uint32_t block_size;
    int removable;
} terra_block_device_t;

int terra_block_list(terra_block_device_t *devices, unsigned long max_devices);
int terra_block_open(const char *name);
int terra_block_close(int handle);

#endif
