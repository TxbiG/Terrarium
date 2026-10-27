
/* TERRA_DEFRAG_VFS_API: mount list and resolver */
/* ------------------------------------------------------------------------- */
/* Read-only mount/volume query API used by maintenance utilities.            */
/* ------------------------------------------------------------------------- */
static int defrag_normalize_query_path(const char *path, char *out, size_t cap) {
    size_t len = 0, i, component_start = 1;
    if (!path || !out || cap < 2 || path[0] != '/')
        return TERRA_FS_ERR_INVAL;
    while (len < cap && path[len])
        ++len;
    if (len == 0 || len >= cap)
        return TERRA_FS_ERR_INVAL;
    for (i = 1; i <= len; ++i) {
        if (i == len || path[i] == '/') {
            size_t n = i - component_start;
            if ((n == 1 && path[component_start] == '.') ||
                (n == 2 && path[component_start] == '.' &&
                 path[component_start + 1] == '.'))
                return TERRA_FS_ERR_INVAL;
            component_start = i + 1;
        }
    }
    return normalize_path(path, out, cap) == 0 ? TERRA_FS_OK : TERRA_FS_ERR_INVAL;
}

static int defrag_mount_prefix_matches(const char *path, const char *prefix) {
    size_t n, path_len, i;
    if (!path || !prefix || !prefix[0])
        return 0;
    n = terra_strlen(prefix);
    path_len = terra_strlen(path);
    if (path_len < n)
        return 0;
    for (i = 0; i < n; ++i) {
        if (path[i] != prefix[i])
            return 0;
    }
    if (path_len == n)
        return 1;
    if (n == 1 && prefix[0] == '/')
        return 1;
    return path[n] == '/';
}

static int defrag_fill_mount_info(size_t slot, terra_fs_mount_info_t *out) {
    const terra_vfs_mount_t *mount;
    const terra_fs_type_t *type;
    terra_fs_block_device_info_t device_info;
    const terra_diskfs_driver_t *driver;
    if (slot >= TERRA_FS_MAX_MOUNTS || !out || !mounts[slot].used)
        return TERRA_FS_ERR_NOENT;
    mount = &mounts[slot];
    type = terra_fs_type_by_id(mount->fs_type);
    if (!type)
        return TERRA_FS_ERR_INVAL;
    terra_mem_zero(out, sizeof(*out));
    terra_copy_string(out->target, sizeof(out->target), mount->path);
    terra_copy_string(out->type, sizeof(out->type), type->name);
    out->mount_slot = (uint32_t)slot;
    out->fs_type_id = mount->fs_type;
    out->block_device_id = mount->block_device;
    out->mount_generation = mount->generation;
    if (mount->readonly || type->status == TERRA_FS_STATUS_READONLY)
        out->flags |= TERRA_FS_MOUNT_INFO_READONLY;
    if (type->flags & TERRA_FS_TYPE_PSEUDO)
        out->flags |= TERRA_FS_MOUNT_INFO_PSEUDO;
    if (type->flags & TERRA_FS_TYPE_DISK)
        out->flags |= TERRA_FS_MOUNT_INFO_DISK;
    if (mount->block_device != 0) {
        int block_status = terra_blockdev_get_info(mount->block_device, &device_info);
        if (block_status != TERRA_FS_OK)
            return TERRA_FS_ERR_NOENT;
        out->flags |= TERRA_FS_MOUNT_INFO_HAS_BLOCK_DEVICE;
        terra_copy_string(out->source, sizeof(out->source), device_info.name);
        out->sector_size_bytes = device_info.sector_size_bytes;
        out->sector_count = device_info.sector_count;
        if (device_info.flags & TERRA_FS_BLOCK_INFO_READONLY) {
            out->flags |= TERRA_FS_MOUNT_INFO_BLOCK_READONLY;
            out->flags |= TERRA_FS_MOUNT_INFO_READONLY;
        }
    }
    driver = (const terra_diskfs_driver_t *)mount->disk_driver;
    if (driver && driver->defrag_volume_info)
        out->flags |= TERRA_FS_MOUNT_INFO_VOLUME_QUERY;
    if (driver && driver->defrag_file_page)
        out->flags |= TERRA_FS_MOUNT_INFO_FILE_QUERY;
    if (driver && driver->defrag_extent_page)
        out->flags |= TERRA_FS_MOUNT_INFO_EXTENT_QUERY;
    if (driver && driver->defrag_free_page)
        out->flags |= TERRA_FS_MOUNT_INFO_FREE_RANGE_QUERY;
    return TERRA_FS_OK;
}

int terra_vfs_mount_list_info(terra_fs_mount_info_t *entries,
                              size_t capacity, uint32_t *out_count) {
    size_t i;
    uint32_t count = 0;
    if (!out_count || (capacity != 0 && !entries))
        return TERRA_FS_ERR_INVAL;
    *out_count = 0;
    mount_table_lock_acquire();
    for (i = 0; i < TERRA_FS_MAX_MOUNTS; ++i)
        if (mounts[i].used)
            ++count;
    *out_count = count;
    if (capacity < (size_t)count) {
        mount_table_lock_release();
        return TERRA_FS_ERR_NOSPC;
    }
    count = 0;
    for (i = 0; i < TERRA_FS_MAX_MOUNTS; ++i) {
        int result;
        if (!mounts[i].used)
            continue;
        result = defrag_fill_mount_info(i, &entries[count]);
        if (result != TERRA_FS_OK) {
            mount_table_lock_release();
            return result;
        }
        ++count;
    }
    mount_table_lock_release();
    *out_count = count;
    return TERRA_FS_OK;
}

int terra_vfs_resolve_mount_info(const char *path,
                                 terra_fs_mount_info_t *out_info) {
    char normalized[TERRA_FS_MAX_PATH];
    size_t i, best = SIZE_MAX, best_len = 0;
    int status;
    if (!out_info)
        return TERRA_FS_ERR_INVAL;
    status = defrag_normalize_query_path(path, normalized, sizeof(normalized));
    if (status != TERRA_FS_OK)
        return status;
    mount_table_lock_acquire();
    for (i = 0; i < TERRA_FS_MAX_MOUNTS; ++i) {
        size_t prefix_len;
        if (!mounts[i].used || !defrag_mount_prefix_matches(normalized, mounts[i].path))
            continue;
        prefix_len = terra_strlen(mounts[i].path);
        if (best == SIZE_MAX || prefix_len > best_len) {
            best = i;
            best_len = prefix_len;
        } else if (prefix_len == best_len) {
            /* Equal-specificity duplicate mount targets are not safe to guess. */
            mount_table_lock_release();
            return TERRA_FS_ERR_BUSY;
        }
    }
    if (best == SIZE_MAX) {
        mount_table_lock_release();
        return TERRA_FS_ERR_NOENT;
    }
    status = defrag_fill_mount_info(best, out_info);
    mount_table_lock_release();
    return status;
}

int terra_vfs_resolve_volume_mount_info(uint64_t requested_volume_id,
                                        terra_fs_mount_info_t *out_info) {
    terra_fs_mount_info_t mounts_info[TERRA_FS_MAX_MOUNTS];
    terra_fs_mount_info_t candidate;
    uint32_t count = 0, i;
    int status, saw_volume_provider = 0, found = 0;
    if (requested_volume_id == 0 || !out_info)
        return TERRA_FS_ERR_INVAL;
    status = terra_vfs_mount_list_info(mounts_info, TERRA_FS_MAX_MOUNTS, &count);
    if (status != TERRA_FS_OK)
        return status;
    for (i = 0; i < count; ++i) {
        terra_defrag_volume_info_t info;
        if (!(mounts_info[i].flags & TERRA_FS_MOUNT_INFO_DISK) ||
            !(mounts_info[i].flags & TERRA_FS_MOUNT_INFO_HAS_BLOCK_DEVICE))
            continue;
        if (!(mounts_info[i].flags & TERRA_FS_MOUNT_INFO_VOLUME_QUERY))
            continue;
        saw_volume_provider = 1;
        terra_mem_zero(&info, sizeof(info));
        status = terra_vfs_defrag_volume_info(mounts_info[i].target, &info);
        if (status != TERRA_FS_OK)
            return status; /* Do not claim uniqueness after an incomplete scan. */
        if (info.volume_id != requested_volume_id)
            continue;
        if (found)
            return TERRA_FS_ERR_BUSY; /* Duplicate provider IDs are ambiguous. */
        candidate = mounts_info[i];
        found = 1;
    }
    if (!found)
        return saw_volume_provider ? TERRA_FS_ERR_NOENT : TERRA_FS_ERR_UNSUPPORTED;

    /* Re-resolve and re-query after scanning all providers. This prevents a
     * mount-slot reuse or provider generation change from returning a stale
     * volume-to-mount association. */
    {
        terra_fs_mount_info_t current;
        terra_defrag_volume_info_t current_volume;
        status = terra_vfs_resolve_mount_info(candidate.target, &current);
        if (status != TERRA_FS_OK)
            return TERRA_FS_ERR_BUSY;
        if (current.mount_slot != candidate.mount_slot ||
            current.mount_generation != candidate.mount_generation ||
            current.block_device_id != candidate.block_device_id ||
            !terra_streq(current.type, candidate.type))
            return TERRA_FS_ERR_BUSY;
        terra_mem_zero(&current_volume, sizeof(current_volume));
        status = terra_vfs_defrag_volume_info(current.target, &current_volume);
        if (status != TERRA_FS_OK || current_volume.volume_id != requested_volume_id)
            return TERRA_FS_ERR_BUSY;
        *out_info = current;
    }
    return TERRA_FS_OK;
}

static int defrag_get_backend(const char *path, uint32_t *out_device,
                              uint64_t *out_mount_generation,
                              const terra_diskfs_driver_t **out_driver,
                              uint8_t out_context[TERRA_FS_DISK_CONTEXT_SIZE]) {
    terra_fs_mount_info_t info;
    const terra_vfs_mount_t *mount;
    const terra_diskfs_driver_t *driver;
    int status;
    if (!out_device || !out_mount_generation || !out_driver || !out_context)
        return TERRA_FS_ERR_INVAL;
    status = terra_vfs_resolve_mount_info(path, &info);
    if (status != TERRA_FS_OK)
        return status;
    if (!(info.flags & TERRA_FS_MOUNT_INFO_HAS_BLOCK_DEVICE))
        return TERRA_FS_ERR_UNSUPPORTED;
    mount_table_lock_acquire();
    if (info.mount_slot >= TERRA_FS_MAX_MOUNTS || !mounts[info.mount_slot].used ||
        mounts[info.mount_slot].generation != info.mount_generation) {
        mount_table_lock_release();
        return TERRA_FS_ERR_BUSY;
    }
    mount = &mounts[info.mount_slot];
    driver = (const terra_diskfs_driver_t *)mount->disk_driver;
    if (!driver) {
        mount_table_lock_release();
        return TERRA_FS_ERR_UNSUPPORTED;
    }
    terra_mem_copy(out_context, mount->disk_context, TERRA_FS_DISK_CONTEXT_SIZE);
    *out_device = mount->block_device;
    *out_mount_generation = mount->generation;
    *out_driver = driver;
    mount_table_lock_release();
    return TERRA_FS_OK;
}

/* Recheck both mount identity and generation after any filesystem callback.
 * A callback may run without the VFS lock; the mount may have been reset or
 * replaced while it was running. */
static int defrag_mount_still_current(const char *path, uint64_t expected_generation,
                                      uint32_t expected_device,
                                      const terra_diskfs_driver_t *expected_driver) {
    terra_fs_mount_info_t info;
    int status = terra_vfs_resolve_mount_info(path, &info);
    if (status != TERRA_FS_OK)
        return status;
    if (info.mount_generation != expected_generation ||
        info.block_device_id != expected_device ||
        !(info.flags & TERRA_FS_MOUNT_INFO_HAS_BLOCK_DEVICE))
        return TERRA_FS_ERR_BUSY;
    mount_table_lock_acquire();
    if (info.mount_slot >= TERRA_FS_MAX_MOUNTS || !mounts[info.mount_slot].used ||
        mounts[info.mount_slot].generation != expected_generation ||
        mounts[info.mount_slot].block_device != expected_device ||
        mounts[info.mount_slot].disk_driver != expected_driver) {
        mount_table_lock_release();
        return TERRA_FS_ERR_BUSY;
    }
    mount_table_lock_release();
    return TERRA_FS_OK;
}

int terra_vfs_defrag_volume_info(const char *path,
                                 terra_defrag_volume_info_t *out_info) {
    uint32_t device;
    uint64_t mount_generation;
    const terra_diskfs_driver_t *driver;
    uint8_t context[TERRA_FS_DISK_CONTEXT_SIZE];
    terra_defrag_volume_info_t info;
    int status;
    if (!out_info)
        return TERRA_FS_ERR_INVAL;
    status = defrag_get_backend(path, &device, &mount_generation, &driver, context);
    (void)mount_generation;
    if (status != TERRA_FS_OK)
        return status;
    if (!driver->defrag_volume_info)
        return TERRA_FS_ERR_UNSUPPORTED;
    terra_mem_zero(&info, sizeof(info));
    status = driver->defrag_volume_info(device, context, &info);
    if (status != TERRA_FS_OK)
        return status;
    status = defrag_mount_still_current(path, mount_generation, device, driver);
    if (status != TERRA_FS_OK)
        return status;
    if (info.abi_version != TERRA_FS_QUERY_ABI_VERSION ||
        info.reserved != 0 || info.volume_id == 0 || info.generation == 0 ||
        info.allocation_unit_bytes == 0 || info.allocation_unit_count == 0 ||
        (info.flags & ~(TERRA_DEFRAG_SNAPSHOT_STABLE |
                        TERRA_DEFRAG_SNAPSHOT_EXTENTS_AUTHORITATIVE |
                        TERRA_DEFRAG_SNAPSHOT_FREE_AUTHORITATIVE |
                        TERRA_DEFRAG_SNAPSHOT_FILES_AUTHORITATIVE)) != 0 ||
        !(info.flags & TERRA_DEFRAG_SNAPSHOT_STABLE))
        return TERRA_FS_ERR_INVAL;
    *out_info = info;
    return TERRA_FS_OK;
}

int terra_vfs_defrag_file_page(const char *path, uint64_t generation,
                               uint64_t cursor,
                               terra_defrag_file_record_t *records,
                               size_t capacity,
                               terra_fs_query_page_t *out_page) {
    uint32_t device;
    uint64_t mount_generation;
    const terra_diskfs_driver_t *driver;
    uint8_t context[TERRA_FS_DISK_CONTEXT_SIZE];
    terra_defrag_volume_info_t volume;
    terra_fs_query_page_t page;
    size_t i;
    int status;
    const uint32_t known_file_flags = TERRA_DEFRAG_FILE_REGULAR |
        TERRA_DEFRAG_FILE_SPARSE | TERRA_DEFRAG_FILE_SHARED |
        TERRA_DEFRAG_FILE_PINNED | TERRA_DEFRAG_FILE_SYSTEM |
        TERRA_DEFRAG_FILE_UNWRITABLE | TERRA_DEFRAG_FILE_OPEN_FOR_WRITE;

    if (!out_page || (capacity != 0 && !records) || capacity > UINT32_MAX)
        return TERRA_FS_ERR_INVAL;
    status = defrag_get_backend(path, &device, &mount_generation, &driver, context);
    if (status != TERRA_FS_OK)
        return status;
    if (!driver->defrag_volume_info || !driver->defrag_file_page)
        return TERRA_FS_ERR_UNSUPPORTED;
    terra_mem_zero(&volume, sizeof(volume));
    status = driver->defrag_volume_info(device, context, &volume);
    if (status != TERRA_FS_OK)
        return status;
    status = defrag_mount_still_current(path, mount_generation, device, driver);
    if (status != TERRA_FS_OK)
        return status;
    if (volume.abi_version != TERRA_FS_QUERY_ABI_VERSION ||
        volume.generation != generation ||
        !(volume.flags & TERRA_DEFRAG_SNAPSHOT_STABLE) ||
        !(volume.flags & TERRA_DEFRAG_SNAPSHOT_FILES_AUTHORITATIVE) ||
        volume.allocation_unit_count == 0)
        return TERRA_FS_ERR_BUSY;

    terra_mem_zero(&page, sizeof(page));
    status = driver->defrag_file_page(device, context, generation, cursor,
                                      records, capacity, &page);
    if (status != TERRA_FS_OK)
        return status;
    if (page.count > capacity || page.has_more > 1 ||
        (page.has_more && page.next_cursor == cursor))
        return TERRA_FS_ERR_INVAL;
    status = defrag_mount_still_current(path, mount_generation, device, driver);
    if (status != TERRA_FS_OK)
        return status;
    for (i = 0; i < page.count; ++i) {
        const terra_defrag_file_record_t *file = &records[i];
        if (file->file_id == 0 || file->reserved != 0 ||
            (file->file_flags & ~known_file_flags) != 0 ||
            (file->logical_block_count == 0 && file->extent_count != 0))
            return TERRA_FS_ERR_INVAL;
        if (i != 0 && records[i - 1].file_id >= file->file_id)
            return TERRA_FS_ERR_INVAL;
    }
    *out_page = page;
    return TERRA_FS_OK;
}

int terra_vfs_defrag_extent_page(const char *path, uint64_t generation,
                                 uint64_t cursor,
                                 terra_defrag_extent_record_t *records,
                                 size_t capacity,
                                 terra_fs_query_page_t *out_page) {
    uint32_t device;
    uint64_t mount_generation;
    const terra_diskfs_driver_t *driver;
    uint8_t context[TERRA_FS_DISK_CONTEXT_SIZE];
    terra_defrag_volume_info_t volume;
    terra_fs_query_page_t page;
    size_t i;
    int status;
    const uint32_t known_file_flags = TERRA_DEFRAG_FILE_REGULAR |
        TERRA_DEFRAG_FILE_SPARSE | TERRA_DEFRAG_FILE_SHARED |
        TERRA_DEFRAG_FILE_PINNED | TERRA_DEFRAG_FILE_SYSTEM |
        TERRA_DEFRAG_FILE_UNWRITABLE | TERRA_DEFRAG_FILE_OPEN_FOR_WRITE;
    if (!out_page || (capacity != 0 && !records) || capacity > UINT32_MAX)
        return TERRA_FS_ERR_INVAL;
    status = defrag_get_backend(path, &device, &mount_generation, &driver, context);
    if (status != TERRA_FS_OK)
        return status;
    if (!driver->defrag_volume_info || !driver->defrag_extent_page)
        return TERRA_FS_ERR_UNSUPPORTED;
    terra_mem_zero(&volume, sizeof(volume));
    status = driver->defrag_volume_info(device, context, &volume);
    if (status != TERRA_FS_OK)
        return status;
    status = defrag_mount_still_current(path, mount_generation, device, driver);
    if (status != TERRA_FS_OK)
        return status;
    if (volume.abi_version != TERRA_FS_QUERY_ABI_VERSION ||
        volume.generation != generation ||
        !(volume.flags & TERRA_DEFRAG_SNAPSHOT_STABLE) ||
        !(volume.flags & TERRA_DEFRAG_SNAPSHOT_EXTENTS_AUTHORITATIVE) ||
        volume.allocation_unit_count == 0)
        return TERRA_FS_ERR_BUSY;
    terra_mem_zero(&page, sizeof(page));
    status = driver->defrag_extent_page(device, context, generation, cursor,
                                        records, capacity, &page);
    if (status != TERRA_FS_OK)
        return status;
    if (page.count > capacity || page.has_more > 1 ||
        (page.has_more && page.next_cursor == cursor) ||
        (capacity != 0 && page.count == 0 && page.has_more && page.next_cursor == cursor))
        return TERRA_FS_ERR_INVAL;
    status = defrag_mount_still_current(path, mount_generation, device, driver);
    if (status != TERRA_FS_OK)
        return status;
    for (i = 0; i < page.count; ++i) {
        const terra_defrag_extent_record_t *e = &records[i];
        uint64_t physical_end, logical_end;
        if (e->file_id == 0 || e->file_block_count == 0 ||
            e->block_count == 0 || e->reserved != 0 ||
            (e->file_flags & ~known_file_flags) != 0 ||
            e->block_count > UINT64_MAX - e->physical_start ||
            e->block_count > UINT64_MAX - e->logical_start)
            return TERRA_FS_ERR_INVAL;
        physical_end = e->physical_start + e->block_count;
        logical_end = e->logical_start + e->block_count;
        if (physical_end > volume.allocation_unit_count ||
            logical_end > e->file_block_count)
            return TERRA_FS_ERR_INVAL;
        if (i != 0) {
            const terra_defrag_extent_record_t *previous = &records[i - 1];
            uint64_t previous_logical_end = previous->logical_start + previous->block_count;
            if (e->file_id < previous->file_id ||
                (e->file_id == previous->file_id &&
                 (e->logical_start < previous_logical_end ||
                  e->file_generation != previous->file_generation ||
                  e->file_block_count != previous->file_block_count ||
                  e->file_flags != previous->file_flags)))
                return TERRA_FS_ERR_INVAL;
        }
    }
    *out_page = page;
    return TERRA_FS_OK;
}

int terra_vfs_defrag_free_range_page(const char *path, uint64_t generation,
                                     uint64_t cursor,
                                     terra_defrag_free_range_record_t *ranges,
                                     size_t capacity,
                                     terra_fs_query_page_t *out_page) {
    uint32_t device;
    uint64_t mount_generation;
    const terra_diskfs_driver_t *driver;
    uint8_t context[TERRA_FS_DISK_CONTEXT_SIZE];
    terra_defrag_volume_info_t volume;
    terra_fs_query_page_t page;
    size_t i;
    int status;
    if (!out_page || (capacity != 0 && !ranges) || capacity > UINT32_MAX)
        return TERRA_FS_ERR_INVAL;
    status = defrag_get_backend(path, &device, &mount_generation, &driver, context);
    if (status != TERRA_FS_OK)
        return status;
    if (!driver->defrag_volume_info || !driver->defrag_free_page)
        return TERRA_FS_ERR_UNSUPPORTED;
    terra_mem_zero(&volume, sizeof(volume));
    status = driver->defrag_volume_info(device, context, &volume);
    if (status != TERRA_FS_OK)
        return status;
    status = defrag_mount_still_current(path, mount_generation, device, driver);
    if (status != TERRA_FS_OK)
        return status;
    if (volume.abi_version != TERRA_FS_QUERY_ABI_VERSION ||
        volume.generation != generation ||
        !(volume.flags & TERRA_DEFRAG_SNAPSHOT_STABLE) ||
        !(volume.flags & TERRA_DEFRAG_SNAPSHOT_FREE_AUTHORITATIVE) ||
        volume.allocation_unit_count == 0)
        return TERRA_FS_ERR_BUSY;
    terra_mem_zero(&page, sizeof(page));
    status = driver->defrag_free_page(device, context, generation, cursor,
                                      ranges, capacity, &page);
    if (status != TERRA_FS_OK)
        return status;
    if (page.count > capacity || page.has_more > 1 ||
        (page.has_more && page.next_cursor == cursor))
        return TERRA_FS_ERR_INVAL;
    status = defrag_mount_still_current(path, mount_generation, device, driver);
    if (status != TERRA_FS_OK)
        return status;
    for (i = 0; i < page.count; ++i) {
        uint64_t end;
        if (ranges[i].block_count == 0 ||
            ranges[i].block_count > UINT64_MAX - ranges[i].physical_start)
            return TERRA_FS_ERR_INVAL;
        end = ranges[i].physical_start + ranges[i].block_count;
        if (end > volume.allocation_unit_count)
            return TERRA_FS_ERR_INVAL;
        if (i != 0) {
            uint64_t previous_end;
            if (ranges[i - 1].block_count > UINT64_MAX - ranges[i - 1].physical_start)
                return TERRA_FS_ERR_INVAL;
            previous_end = ranges[i - 1].physical_start + ranges[i - 1].block_count;
            if (ranges[i].physical_start < previous_end)
                return TERRA_FS_ERR_INVAL;
        }
    }
    *out_page = page;
    return TERRA_FS_OK;
}
