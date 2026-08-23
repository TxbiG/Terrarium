#include "xfs.h"
#include "../diskfs.h"

#define TERRA_XFS_VERSION_MASK 0x000fu
#define TERRA_XFS_VERSION_4 4u
#define TERRA_XFS_VERSION_5 5u

static uint16_t xfs_be16(const uint8_t *p) {
    return ((uint16_t)p[0] << 8) | (uint16_t)p[1];
}

static uint32_t xfs_be32(const uint8_t *p) {
    return ((uint32_t)p[0] << 24) |
           ((uint32_t)p[1] << 16) |
           ((uint32_t)p[2] << 8) |
           (uint32_t)p[3];
}

static uint64 xfs_be64(const uint8_t *p) {
    return ((uint64)xfs_be32(p) << 32) | (uint64)xfs_be32(p + 4);
}

static size_t xfs_strlen(const char *text) {
    size_t len = 0;
    if (!text)
        return 0;
    while (text[len])
        ++len;
    return len;
}

static void xfs_append(char *buffer, size_t size, size_t *used, const char *text) {
    size_t i = 0;
    if (!buffer || !used || !text || size == 0)
        return;
    while (text[i] && *used + 1 < size)
        buffer[(*used)++] = text[i++];
    buffer[*used] = '\0';
}

static void xfs_append_u64(char *buffer, size_t size, size_t *used, uint64 value) {
    char tmp[21];
    size_t pos = 0;
    if (value == 0) {
        xfs_append(buffer, size, used, "0");
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
        xfs_append(buffer, size, used, ch);
    }
}

static int xfs_bytes_equal(const uint8_t *buf, uint32_t offset, const char *text) {
    uint32_t i = 0;
    while (text[i]) {
        if (buf[offset + i] != (uint8_t)text[i])
            return 0;
        ++i;
    }
    return 1;
}

static int xfs_is_power_of_two(uint32_t value) {
    return value != 0 && (value & (value - 1u)) == 0;
}

static int xfs_read_super(uint32_t block_device, uint8_t *sector) {
    const terra_block_device_t *dev = terra_blockdev_get(block_device);
    if (!dev || !sector || dev->sector_size != 512)
        return TERRA_FS_ERR_INVAL;
    return terra_blockdev_read(block_device, 0, 1, sector) == 1
        ? TERRA_FS_OK
        : TERRA_FS_ERR_INVAL;
}

static int xfs_parse_super(uint32_t block_device, const uint8_t *sb, terra_xfs_volume_t *out_volume) {
    uint32_t block_size;
    uint64 data_blocks;
    uint32_t ag_blocks;
    uint32_t ag_count;
    uint16_t version_raw;
    uint16_t version;
    uint16_t sector_size;
    uint16_t inode_size;

    if (!sb || !out_volume)
        return TERRA_FS_ERR_INVAL;
    if (!xfs_bytes_equal(sb, 0, "XFSB"))
        return TERRA_FS_ERR_INVAL;

    block_size = xfs_be32(&sb[4]);
    data_blocks = xfs_be64(&sb[8]);
    ag_blocks = xfs_be32(&sb[84]);
    ag_count = xfs_be32(&sb[88]);
    version_raw = xfs_be16(&sb[100]);
    version = version_raw & TERRA_XFS_VERSION_MASK;
    sector_size = xfs_be16(&sb[104]);
    inode_size = xfs_be16(&sb[106]);

    if (!xfs_is_power_of_two(block_size) || block_size < 512 || block_size > 65536u)
        return TERRA_FS_ERR_INVAL;
    if (!xfs_is_power_of_two(sector_size) || sector_size < 512 || sector_size > block_size)
        return TERRA_FS_ERR_INVAL;
    if (!xfs_is_power_of_two(inode_size) || inode_size < 256 || inode_size > block_size)
        return TERRA_FS_ERR_INVAL;
    if (data_blocks == 0 || ag_blocks == 0 || ag_count == 0)
        return TERRA_FS_ERR_INVAL;
    if (version != TERRA_XFS_VERSION_4 && version != TERRA_XFS_VERSION_5)
        return TERRA_FS_ERR_INVAL;

    out_volume->block_device = block_device;
    out_volume->block_size = block_size;
    out_volume->data_blocks = data_blocks;
    out_volume->realtime_blocks = xfs_be64(&sb[16]);
    out_volume->realtime_extents = xfs_be64(&sb[24]);
    out_volume->log_start = xfs_be64(&sb[48]);
    out_volume->root_inode = xfs_be64(&sb[56]);
    out_volume->realtime_extent_size = xfs_be32(&sb[80]);
    out_volume->allocation_group_blocks = ag_blocks;
    out_volume->allocation_group_count = ag_count;
    out_volume->log_blocks = xfs_be32(&sb[96]);
    out_volume->version = version;
    out_volume->sector_size = sector_size;
    out_volume->inode_size = inode_size;
    out_volume->crc_enabled = version == TERRA_XFS_VERSION_5;
    out_volume->readonly = 1;
    return TERRA_FS_OK;
}

int terra_xfs_probe(uint32_t block_device) {
    uint8_t sector[512];
    terra_xfs_volume_t volume;

    if (xfs_read_super(block_device, sector) != TERRA_FS_OK)
        return TERRA_DISKFS_NO_MATCH;
    return xfs_parse_super(block_device, sector, &volume) == TERRA_FS_OK
        ? TERRA_DISKFS_MATCH
        : TERRA_DISKFS_NO_MATCH;
}

int terra_xfs_mount(uint32_t block_device, terra_xfs_volume_t *out_volume) {
    uint8_t sector[512];

    if (xfs_read_super(block_device, sector) != TERRA_FS_OK)
        return TERRA_FS_ERR_INVAL;
    return xfs_parse_super(block_device, sector, out_volume);
}

int terra_xfs_describe(const terra_xfs_volume_t *volume, char *buffer, size_t size) {
    size_t used = 0;
    if (!volume || !buffer || size == 0)
        return TERRA_FS_ERR_INVAL;
    buffer[0] = '\0';
    xfs_append(buffer, size, &used, "type xfs\nstatus read-only\nblock_size ");
    xfs_append_u64(buffer, size, &used, volume->block_size);
    xfs_append(buffer, size, &used, "\ndata_blocks ");
    xfs_append_u64(buffer, size, &used, volume->data_blocks);
    xfs_append(buffer, size, &used, "\nroot_inode ");
    xfs_append_u64(buffer, size, &used, volume->root_inode);
    xfs_append(buffer, size, &used, "\nallocation_group_count ");
    xfs_append_u64(buffer, size, &used, volume->allocation_group_count);
    xfs_append(buffer, size, &used, "\nlog_start ");
    xfs_append_u64(buffer, size, &used, volume->log_start);
    xfs_append(buffer, size, &used, "\ncrc ");
    xfs_append(buffer, size, &used, volume->crc_enabled ? "yes" : "no");
    xfs_append(buffer, size, &used, "\n");
    return (int)xfs_strlen(buffer);
}

const char *terra_xfs_status(void) {
    return "native-readonly";
}
