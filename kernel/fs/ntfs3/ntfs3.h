#ifndef TERRARIUM_NTFS3_H
#define TERRARIUM_NTFS3_H

#include "../blockdev.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct terra_ntfs3_volume {
    uint32 block_device;
    uint32 bytes_per_sector;
    uint32 sectors_per_cluster;
    uint32 cluster_size;
    uint64 total_sectors;
    uint64 mft_lcn;
    uint64 mft_mirror_lcn;
    uint32 file_record_size;
    uint32 index_record_size;
    uint64 volume_serial;
    int readonly;
} terra_ntfs3_volume_t;

int terra_ntfs3_probe(uint32 block_device);
int terra_ntfs3_mount(uint32 block_device, terra_ntfs3_volume_t *out_volume);
int terra_ntfs3_describe(const terra_ntfs3_volume_t *volume, char *buffer, size_t size);
const char *terra_ntfs3_status(void);

#ifdef __cplusplus
}
#endif

#endif
