#include "ntfs3.h"
#include "../diskfs.h"

static uint16_t ntfs_le16(const uint8_t *p) {
    return (uint16_t)p[0] | ((uint16_t)p[1] << 8);
}

static uint32_t ntfs_le32(const uint8_t *p) {
    return (uint32_t)p[0] |
           ((uint32_t)p[1] << 8) |
           ((uint32_t)p[2] << 16) |
           ((uint32_t)p[3] << 24);
}

static uint64_t ntfs_le64(const uint8_t *p) {
    return (uint64_t)ntfs_le32(p) | ((uint64_t)ntfs_le32(p + 4) << 32);
}

static size_t ntfs_strlen(const char *text) {
    size_t len = 0;
    if (!text)
        return 0;
    while (text[len])
        ++len;
    return len;
}

static void ntfs_append(char *buffer, size_t size, size_t *used, const char *text) {
    size_t i = 0;
    if (!buffer || !used || !text || size == 0)
        return;
    while (text[i] && *used + 1 < size)
        buffer[(*used)++] = text[i++];
    buffer[*used] = '\0';
}

static void ntfs_append_u64(char *buffer, size_t size, size_t *used, uint64_t value) {
    char tmp[21];
    size_t pos = 0;
    if (value == 0) {
        ntfs_append(buffer, size, used, "0");
        return;
    }
    while (value && pos < sizeof(tmp)) {
        tmp[pos++] = (char)('0' + (value % 10));
        value /= 10;
    }
    while (pos > 0) {
        char ch[2];
        ch[0] = tmp[--pos];
        ch[1] = '\0';
        ntfs_append(buffer, size, used, ch);
    }
}

static int ntfs_bytes_equal(const uint8_t *buf, uint32_t offset, const char *text) {
    uint32_t i = 0;
    while (text[i]) {
        if (buf[offset + i] != (uint8_t)text[i])
            return 0;
        ++i;
    }
    return 1;
}

static int ntfs_is_power_of_two(uint32_t value) {
    return value != 0 && (value & (value - 1u)) == 0;
}

static uint32_t ntfs_record_size(uint32_t cluster_size, int8 encoded) {
    if (encoded < 0) {
        uint8_t shift = (uint8_t)(-encoded);
        if (shift >= 31)
            return 0;
        return (uint32_t)1u << shift;
    }
    return cluster_size * (uint32_t)encoded;
}

static int ntfs_read_boot(uint32_t block_device, uint8_t *sector) {
    const terra_block_device_t *dev = terra_blockdev_get(block_device);
    if (!dev || !sector || dev->sector_size != 512)
        return TERRA_FS_ERR_INVAL;
    return terra_blockdev_read(block_device, 0, 1, sector) == 1
        ? TERRA_FS_OK
        : TERRA_FS_ERR_INVAL;
}

static int ntfs_parse_boot(uint32_t block_device, const uint8_t *sector, terra_ntfs3_volume_t *out_volume) {
    uint32_t bytes_per_sector;
    uint32_t sectors_per_cluster;
    uint32_t cluster_size;
    uint64_t total_sectors;
    uint64_t mft_lcn;
    uint64_t mft_mirror_lcn;
    uint32_t file_record_size;
    uint32_t index_record_size;

    if (!sector || !out_volume)
        return TERRA_FS_ERR_INVAL;
    if (sector[510] != 0x55 || sector[511] != 0xAA)
        return TERRA_FS_ERR_INVAL;
    if (!ntfs_bytes_equal(sector, 3, "NTFS    "))
        return TERRA_FS_ERR_INVAL;

    bytes_per_sector = ntfs_le16(&sector[11]);
    sectors_per_cluster = sector[13];
    total_sectors = ntfs_le64(&sector[40]);
    mft_lcn = ntfs_le64(&sector[48]);
    mft_mirror_lcn = ntfs_le64(&sector[56]);

    if (!ntfs_is_power_of_two(bytes_per_sector) || bytes_per_sector < 512 || bytes_per_sector > 4096)
        return TERRA_FS_ERR_INVAL;
    if (!ntfs_is_power_of_two(sectors_per_cluster) || sectors_per_cluster == 0)
        return TERRA_FS_ERR_INVAL;
    if (total_sectors == 0 || mft_lcn == 0)
        return TERRA_FS_ERR_INVAL;

    cluster_size = bytes_per_sector * sectors_per_cluster;
    if (cluster_size == 0 || cluster_size > 1048576u)
        return TERRA_FS_ERR_INVAL;

    file_record_size = ntfs_record_size(cluster_size, (int8)sector[64]);
    index_record_size = ntfs_record_size(cluster_size, (int8)sector[68]);
    if (file_record_size < 256 || index_record_size < 512)
        return TERRA_FS_ERR_INVAL;

    out_volume->block_device = block_device;
    out_volume->bytes_per_sector = bytes_per_sector;
    out_volume->sectors_per_cluster = sectors_per_cluster;
    out_volume->cluster_size = cluster_size;
    out_volume->total_sectors = total_sectors;
    out_volume->mft_lcn = mft_lcn;
    out_volume->mft_mirror_lcn = mft_mirror_lcn;
    out_volume->file_record_size = file_record_size;
    out_volume->index_record_size = index_record_size;
    out_volume->volume_serial = ntfs_le64(&sector[72]);
    out_volume->readonly = 1;
    return TERRA_FS_OK;
}

int terra_ntfs3_probe(uint32_t block_device) {
    uint8_t sector[512];
    terra_ntfs3_volume_t volume;

    if (ntfs_read_boot(block_device, sector) != TERRA_FS_OK)
        return TERRA_DISKFS_NO_MATCH;
    return ntfs_parse_boot(block_device, sector, &volume) == TERRA_FS_OK
        ? TERRA_DISKFS_MATCH
        : TERRA_DISKFS_NO_MATCH;
}

int terra_ntfs3_mount(uint32_t block_device, terra_ntfs3_volume_t *out_volume) {
    uint8_t sector[512];

    if (ntfs_read_boot(block_device, sector) != TERRA_FS_OK)
        return TERRA_FS_ERR_INVAL;
    return ntfs_parse_boot(block_device, sector, out_volume);
}

int terra_ntfs3_describe(const terra_ntfs3_volume_t *volume, char *buffer, size_t size) {
    size_t used = 0;
    if (!volume || !buffer || size == 0)
        return TERRA_FS_ERR_INVAL;
    buffer[0] = '\0';
    ntfs_append(buffer, size, &used, "type ntfs3\nstatus read-only\nbytes_per_sector ");
    ntfs_append_u64(buffer, size, &used, volume->bytes_per_sector);
    ntfs_append(buffer, size, &used, "\nsectors_per_cluster ");
    ntfs_append_u64(buffer, size, &used, volume->sectors_per_cluster);
    ntfs_append(buffer, size, &used, "\ntotal_sectors ");
    ntfs_append_u64(buffer, size, &used, volume->total_sectors);
    ntfs_append(buffer, size, &used, "\nmft_lcn ");
    ntfs_append_u64(buffer, size, &used, volume->mft_lcn);
    ntfs_append(buffer, size, &used, "\nfile_record_size ");
    ntfs_append_u64(buffer, size, &used, volume->file_record_size);
    ntfs_append(buffer, size, &used, "\nindex_record_size ");
    ntfs_append_u64(buffer, size, &used, volume->index_record_size);
    ntfs_append(buffer, size, &used, "\n");
    return (int)ntfs_strlen(buffer);
}

const char *terra_ntfs3_status(void) {
    return "native-readonly";
}
