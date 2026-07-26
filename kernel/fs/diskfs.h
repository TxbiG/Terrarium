#ifndef TERRARIUM_DISKFS_H
#define TERRARIUM_DISKFS_H

#include "blockdev.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum terra_diskfs_probe_result {
    TERRA_DISKFS_NO_MATCH = 0,
    TERRA_DISKFS_MATCH = 1,
} terra_diskfs_probe_result_t;

typedef int (*terra_diskfs_mount_fn)(uint32 block_device, void *context, size_t context_size);
typedef int (*terra_diskfs_describe_fn)(const void *context, char *buffer, size_t size);

typedef struct terra_diskfs_driver {
    const char *name;
    size_t context_size;
    terra_diskfs_mount_fn mount;
    terra_diskfs_describe_fn describe;
} terra_diskfs_driver_t;

int terra_diskfs_probe_fat32(uint32 block_device);
int terra_diskfs_probe_ext2(uint32 block_device);
int terra_diskfs_probe_exfat(uint32 block_device);
int terra_diskfs_probe_ext4(uint32 block_device);
int terra_diskfs_probe_ntfs3(uint32 block_device);
int terra_diskfs_probe_xfs(uint32 block_device);
const char *terra_diskfs_detect(uint32 block_device);
const terra_diskfs_driver_t *terra_diskfs_driver_by_name(const char *name);

#ifdef __cplusplus
}
#endif

#endif
