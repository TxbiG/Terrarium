#include "exfat.h"
#include "../diskfs.h"

static uint16_t exfat_le16(const uint8_t *p) {
    return (uint16_t)p[0] | ((uint16_t)p[1] << 8);
}

static uint32_t exfat_le32(const uint8_t *p) {
    return (uint32_t)p[0] |
           ((uint32_t)p[1] << 8) |
           ((uint32_t)p[2] << 16) |
           ((uint32_t)p[3] << 24);
}

static uint64_t exfat_le64(const uint8_t *p) {
    return (uint64_t)exfat_le32(p) | ((uint64_t)exfat_le32(p + 4) << 32);
}

static size_t exfat_strlen(const char *text) {
    size_t len = 0;
    if (!text)
        return 0;
    while (text[len])
        ++len;
    return len;
}

static void exfat_append(char *buffer, size_t size, size_t *used, const char *text) {
    size_t i = 0;
    if (!buffer || !used || !text || size == 0)
        return;
    while (text[i] && *used + 1 < size)
        buffer[(*used)++] = text[i++];
    buffer[*used] = '\0';
}

static void exfat_append_u64(char *buffer, size_t size, size_t *used, uint64_t value) {
    char tmp[21];
    size_t pos = 0;
    if (value == 0) {
        exfat_append(buffer, size, used, "0");
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
        exfat_append(buffer, size, used, ch);
    }
}

static int exfat_bytes_equal(const uint8_t *buf, uint32_t offset, const char *text) {
    uint32_t i = 0;
    while (text[i]) {
        if (buf[offset + i] != (uint8_t)text[i])
            return 0;
        ++i;
    }
    return 1;
}

static int exfat_read_boot(uint32_t block_device, uint8_t *sector) {
    const terra_block_device_t *dev = terra_blockdev_get(block_device);
    if (!dev || !sector || dev->sector_size != 512)
        return TERRA_FS_ERR_INVAL;
    return terra_blockdev_read(block_device, 0, 1, sector) == 1
        ? TERRA_FS_OK
        : TERRA_FS_ERR_INVAL;
}

static int exfat_parse_boot(uint32_t block_device, const uint8_t *sector, terra_exfat_volume_t *out_volume) {
    uint8_t sector_shift;
    uint8_t cluster_shift;
    uint32_t bytes_per_sector;
    uint32_t sectors_per_cluster;
    uint32_t cluster_size;
    uint32_t cluster_count;
    uint32_t root_dir_cluster;
    uint8_t number_of_fats;

    if (!sector || !out_volume)
        return TERRA_FS_ERR_INVAL;
    if (sector[510] != 0x55 || sector[511] != 0xAA)
        return TERRA_FS_ERR_INVAL;
    if (!exfat_bytes_equal(sector, 3, "EXFAT   "))
        return TERRA_FS_ERR_INVAL;

    sector_shift = sector[108];
    cluster_shift = sector[109];
    number_of_fats = sector[110];
    cluster_count = exfat_le32(&sector[92]);
    root_dir_cluster = exfat_le32(&sector[96]);

    if (sector_shift < 9 || sector_shift > 12 || cluster_shift > 25)
        return TERRA_FS_ERR_INVAL;
    if (number_of_fats == 0 || number_of_fats > 2)
        return TERRA_FS_ERR_INVAL;
    if (cluster_count == 0 || root_dir_cluster < 2 || root_dir_cluster >= cluster_count + 2)
        return TERRA_FS_ERR_INVAL;

    bytes_per_sector = (uint32_t)1u << sector_shift;
    sectors_per_cluster = (uint32_t)1u << cluster_shift;
    cluster_size = bytes_per_sector * sectors_per_cluster;
    if (cluster_size == 0)
        return TERRA_FS_ERR_INVAL;

    out_volume->block_device = block_device;
    out_volume->partition_offset = exfat_le64(&sector[64]);
    out_volume->volume_length = exfat_le64(&sector[72]);
    out_volume->fat_offset = exfat_le32(&sector[80]);
    out_volume->fat_length = exfat_le32(&sector[84]);
    out_volume->cluster_heap_offset = exfat_le32(&sector[88]);
    out_volume->cluster_count = cluster_count;
    out_volume->root_dir_cluster = root_dir_cluster;
    out_volume->volume_serial = exfat_le32(&sector[100]);
    out_volume->fs_revision = exfat_le16(&sector[104]);
    out_volume->volume_flags = exfat_le16(&sector[106]);
    out_volume->bytes_per_sector = bytes_per_sector;
    out_volume->sectors_per_cluster = sectors_per_cluster;
    out_volume->cluster_size = cluster_size;
    out_volume->number_of_fats = number_of_fats;
    out_volume->readonly = 1;
    return TERRA_FS_OK;
}

int terra_exfat_probe(uint32_t block_device) {
    uint8_t sector[512];
    terra_exfat_volume_t volume;

    if (exfat_read_boot(block_device, sector) != TERRA_FS_OK)
        return TERRA_DISKFS_NO_MATCH;
    return exfat_parse_boot(block_device, sector, &volume) == TERRA_FS_OK
        ? TERRA_DISKFS_MATCH
        : TERRA_DISKFS_NO_MATCH;
}

int terra_exfat_mount(uint32_t block_device, terra_exfat_volume_t *out_volume) {
    uint8_t sector[512];

    if (exfat_read_boot(block_device, sector) != TERRA_FS_OK)
        return TERRA_FS_ERR_INVAL;
    return exfat_parse_boot(block_device, sector, out_volume);
}

int terra_exfat_describe(const terra_exfat_volume_t *volume, char *buffer, size_t size) {
    size_t used = 0;
    if (!volume || !buffer || size == 0)
        return TERRA_FS_ERR_INVAL;
    buffer[0] = '\0';
    exfat_append(buffer, size, &used, "type exfat\nstatus read-only\nbytes_per_sector ");
    exfat_append_u64(buffer, size, &used, volume->bytes_per_sector);
    exfat_append(buffer, size, &used, "\nsectors_per_cluster ");
    exfat_append_u64(buffer, size, &used, volume->sectors_per_cluster);
    exfat_append(buffer, size, &used, "\ncluster_count ");
    exfat_append_u64(buffer, size, &used, volume->cluster_count);
    exfat_append(buffer, size, &used, "\nroot_dir_cluster ");
    exfat_append_u64(buffer, size, &used, volume->root_dir_cluster);
    exfat_append(buffer, size, &used, "\nfat_offset ");
    exfat_append_u64(buffer, size, &used, volume->fat_offset);
    exfat_append(buffer, size, &used, "\nfat_length ");
    exfat_append_u64(buffer, size, &used, volume->fat_length);
    exfat_append(buffer, size, &used, "\n");
    return (int)exfat_strlen(buffer);
}

const char *terra_exfat_status(void) {
    return "native-readonly";
}
