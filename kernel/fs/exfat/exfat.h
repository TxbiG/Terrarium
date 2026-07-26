#ifndef TERRARIUM_EXFAT_H
#define TERRARIUM_EXFAT_H

#include "../blockdev.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct terra_exfat_volume {
    uint32 block_device;
    uint64 partition_offset;
    uint64 volume_length;
    uint32 fat_offset;
    uint32 fat_length;
    uint32 cluster_heap_offset;
    uint32 cluster_count;
    uint32 root_dir_cluster;
    uint32 volume_serial;
    uint16 fs_revision;
    uint16 volume_flags;
    uint32 bytes_per_sector;
    uint32 sectors_per_cluster;
    uint32 cluster_size;
    uint8 number_of_fats;
    int readonly;
} terra_exfat_volume_t;

int terra_exfat_probe(uint32 block_device);
int terra_exfat_mount(uint32 block_device, terra_exfat_volume_t *out_volume);
int terra_exfat_describe(const terra_exfat_volume_t *volume, char *buffer, size_t size);
const char *terra_exfat_status(void);

#ifdef __cplusplus
}
#endif

#endif
