#include "ext4.h"
#include "../diskfs.h"

static uint16_t ext4_le16(const uint8_t *p) {
    return (uint16_t)p[0] | ((uint16_t)p[1] << 8);
}

static uint32_t ext4_le32(const uint8_t *p) {
    return (uint32_t)p[0] |
           ((uint32_t)p[1] << 8) |
           ((uint32_t)p[2] << 16) |
           ((uint32_t)p[3] << 24);
}

static uint64_t ext4_le64_pair(const uint8_t *lo, const uint8_t *hi) {
    return (uint64_t)ext4_le32(lo) | ((uint64_t)ext4_le32(hi) << 32);
}

static size_t ext4_strlen(const char *text) {
    size_t len = 0;
    if (!text)
        return 0;
    while (text[len])
        ++len;
    return len;
}

static void ext4_append(char *buffer, size_t size, size_t *used, const char *text) {
    size_t i = 0;
    if (!buffer || !used || !text || size == 0)
        return;
    while (text[i] && *used + 1 < size)
        buffer[(*used)++] = text[i++];
    buffer[*used] = '\0';
}

static void ext4_append_u64(char *buffer, size_t size, size_t *used, uint64_t value) {
    char tmp[21];
    size_t pos = 0;
    if (value == 0) {
        ext4_append(buffer, size, used, "0");
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
        ext4_append(buffer, size, used, ch);
    }
}

static int ext4_read_super(uint32_t block_device, uint8_t *superblock) {
    const terra_block_device_t *dev = terra_blockdev_get(block_device);
    if (!dev || !superblock || dev->sector_size != 512)
        return TERRA_FS_ERR_INVAL;
    return terra_blockdev_read(block_device, 2, 2, superblock) == 2
        ? TERRA_FS_OK
        : TERRA_FS_ERR_INVAL;
}

static int ext4_parse_super(uint32_t block_device, const uint8_t *sb, terra_ext4_volume_t *out_volume) {
    uint32_t log_block_size;
    uint32_t feature_incompat;
    uint32_t feature_ro_compat;
    uint32_t block_size;
    uint32_t ext4_incompat;
    uint32_t ext4_ro_compat;

    if (!sb || !out_volume)
        return TERRA_FS_ERR_INVAL;
    if (ext4_le16(&sb[56]) != 0xEF53)
        return TERRA_FS_ERR_INVAL;

    log_block_size = ext4_le32(&sb[24]);
    if (log_block_size > 6)
        return TERRA_FS_ERR_INVAL;

    block_size = 1024u << log_block_size;
    feature_incompat = ext4_le32(&sb[96]);
    feature_ro_compat = ext4_le32(&sb[100]);
    ext4_incompat = TERRA_EXT4_FEATURE_INCOMPAT_EXTENTS |
                    TERRA_EXT4_FEATURE_INCOMPAT_64BIT |
                    TERRA_EXT4_FEATURE_INCOMPAT_FLEX_BG;
    ext4_ro_compat = TERRA_EXT4_FEATURE_RO_COMPAT_HUGE_FILE |
                     TERRA_EXT4_FEATURE_RO_COMPAT_DIR_NLINK |
                     TERRA_EXT4_FEATURE_RO_COMPAT_EXTRA_ISIZE |
                     TERRA_EXT4_FEATURE_RO_COMPAT_METADATA_CSUM;

    out_volume->block_device = block_device;
    out_volume->inode_count = ext4_le32(&sb[0]);
    out_volume->block_count = ext4_le64_pair(&sb[4], &sb[336]);
    out_volume->block_size = block_size;
    out_volume->blocks_per_group = ext4_le32(&sb[32]);
    out_volume->inodes_per_group = ext4_le32(&sb[40]);
    out_volume->feature_compat = ext4_le32(&sb[92]);
    out_volume->feature_incompat = feature_incompat;
    out_volume->feature_ro_compat = feature_ro_compat;
    out_volume->inode_size = ext4_le16(&sb[88]);
    out_volume->state = ext4_le16(&sb[58]);
    out_volume->has_extents = (feature_incompat & TERRA_EXT4_FEATURE_INCOMPAT_EXTENTS) != 0;
    out_volume->is_ext4 = ((feature_incompat & ext4_incompat) != 0) ||
                          ((feature_ro_compat & ext4_ro_compat) != 0);
    out_volume->readonly = 1;

    if (out_volume->inode_count == 0 || out_volume->block_count == 0)
        return TERRA_FS_ERR_INVAL;
    if (out_volume->blocks_per_group == 0 || out_volume->inodes_per_group == 0)
        return TERRA_FS_ERR_INVAL;

    return TERRA_FS_OK;
}

int terra_ext4_probe(uint32_t block_device) {
    uint8_t superblock[1024];
    terra_ext4_volume_t volume;

    if (ext4_read_super(block_device, superblock) != TERRA_FS_OK)
        return TERRA_DISKFS_NO_MATCH;
    if (ext4_parse_super(block_device, superblock, &volume) != TERRA_FS_OK)
        return TERRA_DISKFS_NO_MATCH;
    return volume.is_ext4 ? TERRA_DISKFS_MATCH : TERRA_DISKFS_NO_MATCH;
}

int terra_ext4_mount(uint32_t block_device, terra_ext4_volume_t *out_volume) {
    uint8_t superblock[1024];

    if (ext4_read_super(block_device, superblock) != TERRA_FS_OK)
        return TERRA_FS_ERR_INVAL;
    if (ext4_parse_super(block_device, superblock, out_volume) != TERRA_FS_OK)
        return TERRA_FS_ERR_INVAL;
    return out_volume->is_ext4 ? TERRA_FS_OK : TERRA_FS_ERR_UNSUPPORTED;
}

int terra_ext4_describe(const terra_ext4_volume_t *volume, char *buffer, size_t size) {
    size_t used = 0;
    if (!volume || !buffer || size == 0)
        return TERRA_FS_ERR_INVAL;
    buffer[0] = '\0';
    ext4_append(buffer, size, &used, "type ext4\nstatus read-only\nblock_size ");
    ext4_append_u64(buffer, size, &used, volume->block_size);
    ext4_append(buffer, size, &used, "\nblock_count ");
    ext4_append_u64(buffer, size, &used, volume->block_count);
    ext4_append(buffer, size, &used, "\ninode_count ");
    ext4_append_u64(buffer, size, &used, volume->inode_count);
    ext4_append(buffer, size, &used, "\nblocks_per_group ");
    ext4_append_u64(buffer, size, &used, volume->blocks_per_group);
    ext4_append(buffer, size, &used, "\ninodes_per_group ");
    ext4_append_u64(buffer, size, &used, volume->inodes_per_group);
    ext4_append(buffer, size, &used, "\nextents ");
    ext4_append(buffer, size, &used, volume->has_extents ? "yes" : "no");
    ext4_append(buffer, size, &used, "\n");
    return (int)ext4_strlen(buffer);
}

const char *terra_ext4_status(void) {
    return "native-readonly";
}
