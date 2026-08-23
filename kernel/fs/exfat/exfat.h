#ifndef TERRARIUM_EXFAT_H
#define TERRARIUM_EXFAT_H

#include "../blockdev.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct terra_exfat_volume {
    uint32_t block_device;
    uint64_t partition_offset;
    uint64_t volume_length;
    uint32_t fat_offset;
    uint32_t fat_length;
    uint32_t cluster_heap_offset;
    uint32_t cluster_count;
    uint32_t root_dir_cluster;
    uint32_t volume_serial;
    uint16_t fs_revision;
    uint16_t volume_flags;
    uint32_t bytes_per_sector;
    uint32_t sectors_per_cluster;
    uint32_t cluster_size;
    uint8_t number_of_fats;
    int readonly;
} terra_exfat_volume_t;

int terra_exfat_probe(uint32_t block_device);
int terra_exfat_mount(uint32_t block_device, terra_exfat_volume_t *out_volume);
int terra_exfat_describe(const terra_exfat_volume_t *volume, char *buffer, size_t size);
const char *terra_exfat_status(void);

#ifdef __cplusplus
}
#endif

#endif
