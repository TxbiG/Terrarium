#ifndef TERRARIUM_XFS_H
#define TERRARIUM_XFS_H

#include "../blockdev.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct terra_xfs_volume {
    uint32 block_device;
    uint32 block_size;
    uint64 data_blocks;
    uint64 realtime_blocks;
    uint64 realtime_extents;
    uint64 log_start;
    uint64 root_inode;
    uint32 realtime_extent_size;
    uint32 allocation_group_blocks;
    uint32 allocation_group_count;
    uint32 log_blocks;
    uint16 version;
    uint16 sector_size;
    uint16 inode_size;
    int crc_enabled;
    int readonly;
} terra_xfs_volume_t;

int terra_xfs_probe(uint32 block_device);
int terra_xfs_mount(uint32 block_device, terra_xfs_volume_t *out_volume);
int terra_xfs_describe(const terra_xfs_volume_t *volume, char *buffer, size_t size);
const char *terra_xfs_status(void);

#ifdef __cplusplus
}
#endif

#endif
