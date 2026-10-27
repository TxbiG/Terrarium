
/* Read-only metadata APIs used by VFS volume inspection and defrag planning. */
static int terra_blockdev_fill_info(const terra_block_device_t *dev,
                                    terra_fs_block_device_info_t *out) {
    if (!dev || !out || !dev->used)
        return TERRA_FS_ERR_INVAL;
    block_zero(out, sizeof(*out));
    out->id = dev->id;
    out->sector_size_bytes = dev->sector_size;
    out->sector_count = dev->sector_count;
    block_copy_string(out->name, sizeof(out->name), dev->name);
    if (dev->readonly || !dev->write)
        out->flags |= TERRA_FS_BLOCK_INFO_READONLY;
    if (dev->sector_count != 0 && dev->sector_size != 0 &&
        dev->sector_count > UINT64_MAX / (uint64_t)dev->sector_size) {
        out->flags |= TERRA_FS_BLOCK_INFO_BYTE_SIZE_OVERFLOW;
        out->total_bytes = 0;
    } else {
        out->total_bytes = dev->sector_count * (uint64_t)dev->sector_size;
    }
    return TERRA_FS_OK;
}

int terra_blockdev_get_info(uint32_t id, terra_fs_block_device_info_t *out_info) {
    size_t i;
    int result = TERRA_FS_ERR_NOENT;
    if (!out_info || id == 0)
        return TERRA_FS_ERR_INVAL;
    blockdev_lock_acquire();
    for (i = 0; i < TERRA_BLOCK_MAX_DEVICES; ++i) {
        if (block_devices[i].used && block_devices[i].id == id) {
            result = terra_blockdev_fill_info(&block_devices[i], out_info);
            break;
        }
    }
    blockdev_lock_release();
    return result;
}

int terra_blockdev_list_info(terra_fs_block_device_info_t *entries,
                             size_t capacity, uint32_t *out_count) {
    size_t i;
    uint32_t count = 0;
    if (!out_count || (capacity != 0 && !entries))
        return TERRA_FS_ERR_INVAL;
    *out_count = 0;
    blockdev_lock_acquire();
    for (i = 0; i < TERRA_BLOCK_MAX_DEVICES; ++i)
        if (block_devices[i].used)
            ++count;
    *out_count = count;
    if (capacity < (size_t)count) {
        blockdev_lock_release();
        return TERRA_FS_ERR_NOSPC;
    }
    count = 0;
    for (i = 0; i < TERRA_BLOCK_MAX_DEVICES; ++i) {
        if (!block_devices[i].used)
            continue;
        if (terra_blockdev_fill_info(&block_devices[i], &entries[count]) != TERRA_FS_OK) {
            blockdev_lock_release();
            return TERRA_FS_ERR_INVAL;
        }
        ++count;
    }
    blockdev_lock_release();
    *out_count = count;
    return TERRA_FS_OK;
}
