#ifndef TERRARIUM_NTFS3_H
#define TERRARIUM_NTFS3_H

#include "../blockdev.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct terra_ntfs3_volume {
    uint32_t block_device;
    uint32_t bytes_per_sector;
    uint32_t sectors_per_cluster;
    uint32_t cluster_size;
    uint64_t total_sectors;
    uint64_t mft_lcn;
    uint64_t mft_mirror_lcn;
    uint32_t file_record_size;
    uint32_t index_record_size;
    uint64_t volume_serial;
    int readonly;
} terra_ntfs3_volume_t;

int terra_ntfs3_probe(uint32_t block_device);
int terra_ntfs3_mount(uint32_t block_device, terra_ntfs3_volume_t *out_volume);
int terra_ntfs3_describe(const terra_ntfs3_volume_t *volume, char *buffer, size_t size);
const char *terra_ntfs3_status(void);

#ifdef __cplusplus
}
#endif

#endif
