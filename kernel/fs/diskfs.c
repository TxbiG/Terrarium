#include "diskfs.h"
#include "exfat/exfat.h"
#include "ext4/ext4.h"
#include "ntfs3/ntfs3.h"
#include "xfs/xfs.h"

static uint16_t le16(const uint8 *p) {
    return (uint16_t)p[0] | ((uint16_t)p[1] << 8);
}

static uint32_t le32(const uint8 *p) {
    return (uint32_t)p[0] |
           ((uint32_t)p[1] << 8) |
           ((uint32_t)p[2] << 16) |
           ((uint32_t)p[3] << 24);
}

static int bytes_equal(const uint8 *buf, uint32_t offset, const char *text) {
    uint32_t i = 0;
    while (text[i]) {
        if (buf[offset + i] != (uint8)text[i])
            return 0;
        ++i;
    }
    return 1;
}

static int diskfs_streq(const char *left, const char *right) {
    uint32_t i = 0;
    if (!left || !right)
        return 0;
    while (left[i] && right[i]) {
        if (left[i] != right[i])
            return 0;
        ++i;
    }
    return left[i] == right[i];
}

static int read_sector(uint32_t block_device, uint64 lba, uint8 *sector) {
    const terra_block_device_t *dev = terra_blockdev_get(block_device);
    if (!dev || !sector || dev->sector_size != 512)
        return TERRA_FS_ERR_INVAL;
    return terra_blockdev_read(block_device, lba, 1, sector) == 1
        ? TERRA_FS_OK
        : TERRA_FS_ERR_INVAL;
}

static int mount_exfat(uint32_t block_device, void *context, size_t context_size) {
    if (!context || context_size < sizeof(terra_exfat_volume_t))
        return TERRA_FS_ERR_INVAL;
    return terra_exfat_mount(block_device, (terra_exfat_volume_t *)context);
}

static int describe_exfat(const void *context, char *buffer, size_t size) {
    return terra_exfat_describe((const terra_exfat_volume_t *)context, buffer, size);
}

static int mount_ext4(uint32_t block_device, void *context, size_t context_size) {
    if (!context || context_size < sizeof(terra_ext4_volume_t))
        return TERRA_FS_ERR_INVAL;
    return terra_ext4_mount(block_device, (terra_ext4_volume_t *)context);
}

static int describe_ext4(const void *context, char *buffer, size_t size) {
    return terra_ext4_describe((const terra_ext4_volume_t *)context, buffer, size);
}

static int mount_ntfs3(uint32_t block_device, void *context, size_t context_size) {
    if (!context || context_size < sizeof(terra_ntfs3_volume_t))
        return TERRA_FS_ERR_INVAL;
    return terra_ntfs3_mount(block_device, (terra_ntfs3_volume_t *)context);
}

static int describe_ntfs3(const void *context, char *buffer, size_t size) {
    return terra_ntfs3_describe((const terra_ntfs3_volume_t *)context, buffer, size);
}

static int mount_xfs(uint32_t block_device, void *context, size_t context_size) {
    if (!context || context_size < sizeof(terra_xfs_volume_t))
        return TERRA_FS_ERR_INVAL;
    return terra_xfs_mount(block_device, (terra_xfs_volume_t *)context);
}

static int describe_xfs(const void *context, char *buffer, size_t size) {
    return terra_xfs_describe((const terra_xfs_volume_t *)context, buffer, size);
}

static const terra_diskfs_driver_t diskfs_drivers[] = {
    { "exfat", sizeof(terra_exfat_volume_t), mount_exfat, describe_exfat },
    { "ext4", sizeof(terra_ext4_volume_t), mount_ext4, describe_ext4 },
    { "ntfs3", sizeof(terra_ntfs3_volume_t), mount_ntfs3, describe_ntfs3 },
    { "xfs", sizeof(terra_xfs_volume_t), mount_xfs, describe_xfs },
};

int terra_diskfs_probe_fat32(uint32_t block_device) {
    uint8 sector[512];
    uint16_t bytes_per_sector;
    uint8 sectors_per_cluster;
    uint16_t reserved_sectors;
    uint8 fats;
    uint32_t fat_size_32;
    uint32_t root_cluster;

    if (read_sector(block_device, 0, sector) != TERRA_FS_OK)
        return TERRA_DISKFS_NO_MATCH;

    if (sector[510] != 0x55 || sector[511] != 0xAA)
        return TERRA_DISKFS_NO_MATCH;

    bytes_per_sector = le16(&sector[11]);
    sectors_per_cluster = sector[13];
    reserved_sectors = le16(&sector[14]);
    fats = sector[16];
    fat_size_32 = le32(&sector[36]);
    root_cluster = le32(&sector[44]);

    if (bytes_per_sector == 0 || sectors_per_cluster == 0 || reserved_sectors == 0)
        return TERRA_DISKFS_NO_MATCH;
    if (fats == 0 || fat_size_32 == 0 || root_cluster < 2)
        return TERRA_DISKFS_NO_MATCH;
    if (!bytes_equal(sector, 82, "FAT32"))
        return TERRA_DISKFS_NO_MATCH;

    return TERRA_DISKFS_MATCH;
}

int terra_diskfs_probe_ext2(uint32_t block_device) {
    uint8 sector[512];
    if (read_sector(block_device, 2, sector) != TERRA_FS_OK)
        return TERRA_DISKFS_NO_MATCH;
    return le16(&sector[56]) == 0xEF53 ? TERRA_DISKFS_MATCH : TERRA_DISKFS_NO_MATCH;
}

int terra_diskfs_probe_exfat(uint32_t block_device) {
    return terra_exfat_probe(block_device);
}

int terra_diskfs_probe_ext4(uint32_t block_device) {
    return terra_ext4_probe(block_device);
}

int terra_diskfs_probe_ntfs3(uint32_t block_device) {
    return terra_ntfs3_probe(block_device);
}

int terra_diskfs_probe_xfs(uint32_t block_device) {
    return terra_xfs_probe(block_device);
}

const char *terra_diskfs_detect(uint32_t block_device) {
    if (terra_diskfs_probe_fat32(block_device) == TERRA_DISKFS_MATCH)
        return "fat32";
    if (terra_diskfs_probe_exfat(block_device) == TERRA_DISKFS_MATCH)
        return "exfat";
    if (terra_diskfs_probe_ntfs3(block_device) == TERRA_DISKFS_MATCH)
        return "ntfs3";
    if (terra_diskfs_probe_xfs(block_device) == TERRA_DISKFS_MATCH)
        return "xfs";
    if (terra_diskfs_probe_ext4(block_device) == TERRA_DISKFS_MATCH)
        return "ext4";
    if (terra_diskfs_probe_ext2(block_device) == TERRA_DISKFS_MATCH)
        return "ext2";
    return 0;
}

const terra_diskfs_driver_t *terra_diskfs_driver_by_name(const char *name) {
    uint32_t i;
    for (i = 0; i < (uint32_t)(sizeof(diskfs_drivers) / sizeof(diskfs_drivers[0])); ++i) {
        if (diskfs_streq(diskfs_drivers[i].name, name))
            return &diskfs_drivers[i];
    }
    return 0;
}
