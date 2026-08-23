#include "blockdev.h"

static terra_block_device_t block_devices[TERRA_BLOCK_MAX_DEVICES];
static uint32_t next_block_id = 1;

static size_t block_strlen(const char *value) {
    size_t len = 0;
    if (!value)
        return 0;
    while (value[len])
        ++len;
    return len;
}

static void block_zero(void *dst, size_t len) {
    uint8 *out = (uint8 *)dst;
    size_t i;
    if (!dst)
        return;
    for (i = 0; i < len; ++i)
        out[i] = 0;
}

static void block_copy_string(char *dst, size_t cap, const char *src) {
    size_t i = 0;
    if (!dst || cap == 0)
        return;
    if (!src) {
        dst[0] = '\0';
        return;
    }
    while (src[i] && i + 1 < cap) {
        dst[i] = src[i];
        ++i;
    }
    dst[i] = '\0';
}

void terra_blockdev_reset(void) {
    block_zero(block_devices, sizeof(block_devices));
    next_block_id = 1;
}

int terra_blockdev_register(
    const char *name,
    uint32_t sector_size,
    uint64 sector_count,
    terra_block_read_t read,
    terra_block_write_t write,
    void *ctx,
    int readonly) {
    size_t i;

    if (!name || block_strlen(name) == 0 || sector_size == 0 || !read)
        return TERRA_FS_ERR_INVAL;

    for (i = 0; i < TERRA_BLOCK_MAX_DEVICES; ++i) {
        if (!block_devices[i].used) {
            block_devices[i].used = 1;
            block_devices[i].id = next_block_id++;
            block_copy_string(block_devices[i].name, sizeof(block_devices[i].name), name);
            block_devices[i].sector_size = sector_size;
            block_devices[i].sector_count = sector_count;
            block_devices[i].read = read;
            block_devices[i].write = write;
            block_devices[i].ctx = ctx;
            block_devices[i].readonly = readonly || !write;
            return (int)block_devices[i].id;
        }
    }

    return TERRA_FS_ERR_NOSPC;
}

const terra_block_device_t *terra_blockdev_get(uint32_t id) {
    size_t i;
    for (i = 0; i < TERRA_BLOCK_MAX_DEVICES; ++i) {
        if (block_devices[i].used && block_devices[i].id == id)
            return &block_devices[i];
    }
    return 0;
}

ssize_t terra_blockdev_read(uint32_t id, uint64 lba, uint32_t sectors, void *buffer) {
    const terra_block_device_t *dev = terra_blockdev_get(id);
    if (!dev || !buffer || sectors == 0)
        return TERRA_FS_ERR_INVAL;
    if (dev->sector_count && lba + sectors > dev->sector_count)
        return TERRA_FS_ERR_INVAL;
    return dev->read(dev->ctx, lba, sectors, buffer);
}

ssize_t terra_blockdev_write(uint32_t id, uint64 lba, uint32_t sectors, const void *buffer) {
    const terra_block_device_t *dev = terra_blockdev_get(id);
    if (!dev || !buffer || sectors == 0)
        return TERRA_FS_ERR_INVAL;
    if (dev->readonly || !dev->write)
        return TERRA_FS_ERR_RDONLY;
    if (dev->sector_count && lba + sectors > dev->sector_count)
        return TERRA_FS_ERR_INVAL;
    return dev->write(dev->ctx, lba, sectors, buffer);
}
