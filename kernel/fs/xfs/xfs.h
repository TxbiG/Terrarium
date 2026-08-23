#ifndef TERRARIUM_XFS_H
#define TERRARIUM_XFS_H

#include "../blockdev.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct terra_xfs_volume {
    uint32_t block_device;
    uint32_t block_size;
    uint64_t data_blocks;
    uint64_t realtime_blocks;
    uint64_t realtime_extents;
    uint64_t log_start;
    uint64_t root_inode;
    uint32_t realtime_extent_size;
    uint32_t allocation_group_blocks;
    uint32_t allocation_group_count;
    uint32_t log_blocks;
    uint16_t version;
    uint16_t sector_size;
    uint16_t inode_size;
    int crc_enabled;
    int readonly;
} terra_xfs_volume_t;

int terra_xfs_probe(uint32_t block_device);
int terra_xfs_mount(uint32_t block_device, terra_xfs_volume_t *out_volume);
int terra_xfs_describe(const terra_xfs_volume_t *volume, char *buffer, size_t size);
const char *terra_xfs_status(void);

#ifdef __cplusplus
}
#endif

#endif
