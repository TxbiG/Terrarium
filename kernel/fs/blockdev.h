#ifndef TERRARIUM_BLOCKDEV_H
#define TERRARIUM_BLOCKDEV_H

#include "fs.h"

#ifdef __cplusplus
extern "C" {
#endif

#define TERRA_BLOCK_MAX_DEVICES 16
#define TERRA_BLOCK_MAX_NAME 32

typedef ssize_t (*terra_block_read_t)(void *ctx, uint64 lba, uint32 sectors, void *buffer);
typedef ssize_t (*terra_block_write_t)(void *ctx, uint64 lba, uint32 sectors, const void *buffer);

typedef struct terra_block_device {
    uint32 id;
    char name[TERRA_BLOCK_MAX_NAME];
    uint32 sector_size;
    uint64 sector_count;
    terra_block_read_t read;
    terra_block_write_t write;
    void *ctx;
    int readonly;
    int used;
} terra_block_device_t;

void terra_blockdev_reset(void);
int terra_blockdev_register(
    const char *name,
    uint32 sector_size,
    uint64 sector_count,
    terra_block_read_t read,
    terra_block_write_t write,
    void *ctx,
    int readonly);
const terra_block_device_t *terra_blockdev_get(uint32 id);
ssize_t terra_blockdev_read(uint32 id, uint64 lba, uint32 sectors, void *buffer);
ssize_t terra_blockdev_write(uint32 id, uint64 lba, uint32 sectors, const void *buffer);

#ifdef __cplusplus
}
#endif

#endif
