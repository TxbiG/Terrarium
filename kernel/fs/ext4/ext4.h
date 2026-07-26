#ifndef TERRARIUM_EXT4_H
#define TERRARIUM_EXT4_H

#include "../blockdev.h"

#ifdef __cplusplus
extern "C" {
#endif

#define TERRA_EXT4_FEATURE_INCOMPAT_EXTENTS 0x00000040u
#define TERRA_EXT4_FEATURE_INCOMPAT_64BIT 0x00000080u
#define TERRA_EXT4_FEATURE_INCOMPAT_FLEX_BG 0x00000200u
#define TERRA_EXT4_FEATURE_RO_COMPAT_HUGE_FILE 0x00000008u
#define TERRA_EXT4_FEATURE_RO_COMPAT_DIR_NLINK 0x00000020u
#define TERRA_EXT4_FEATURE_RO_COMPAT_EXTRA_ISIZE 0x00000040u
#define TERRA_EXT4_FEATURE_RO_COMPAT_METADATA_CSUM 0x00000400u

typedef struct terra_ext4_volume {
    uint32 block_device;
    uint32 inode_count;
    uint64 block_count;
    uint32 block_size;
    uint32 blocks_per_group;
    uint32 inodes_per_group;
    uint32 feature_compat;
    uint32 feature_incompat;
    uint32 feature_ro_compat;
    uint16 inode_size;
    uint16 state;
    int has_extents;
    int is_ext4;
    int readonly;
} terra_ext4_volume_t;

int terra_ext4_probe(uint32 block_device);
int terra_ext4_mount(uint32 block_device, terra_ext4_volume_t *out_volume);
int terra_ext4_describe(const terra_ext4_volume_t *volume, char *buffer, size_t size);
const char *terra_ext4_status(void);

#ifdef __cplusplus
}
#endif

#endif
