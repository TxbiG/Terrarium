#include "defrag.h"
#include "../runtime/syscall.h"
#include "../status.h"

#include <limits.h>

static int result_to_status(long result) {
    if (result < (long)INT_MIN || result > (long)INT_MAX)
        return TERRA_STATUS_IO;
    return (int)result;
}

int terra_fs_mount_info_list(terra_fs_mount_info_t *entries,
                             size_t capacity, uint32_t *out_count) {
    if (!out_count || (capacity != 0 && !entries))
        return TERRA_STATUS_INVAL;
    *out_count = 0;
    return result_to_status(terra_syscall(TERRA_SYSCALL_FS_MOUNT_LIST,
        (uintptr_t)entries, (uintptr_t)capacity, (uintptr_t)out_count, 0, 0, 0));
}

int terra_fs_resolve_mount_info(const char *path,
                                terra_fs_mount_info_t *out_info) {
    if (!path || !out_info)
        return TERRA_STATUS_INVAL;
    return result_to_status(terra_syscall(TERRA_SYSCALL_FS_MOUNT_RESOLVE,
        (uintptr_t)path, (uintptr_t)out_info, 0, 0, 0, 0));
}

int terra_fs_resolve_volume_mount_info(uint64_t volume_id,
                                       terra_fs_mount_info_t *out_info) {
    if (volume_id == 0 || !out_info)
        return TERRA_STATUS_INVAL;
    return result_to_status(terra_syscall(TERRA_SYSCALL_FS_RESOLVE_VOLUME, 
        (uintptr_t)volume_id, (uintptr_t)out_info, 0, 0, 0, 0));
}

int terra_fs_block_device_list(terra_fs_block_device_info_t *devices,
                               size_t capacity, uint32_t *out_count) {
    if (!out_count || (capacity != 0 && !devices))
        return TERRA_STATUS_INVAL;
    *out_count = 0;
    return result_to_status(terra_syscall(TERRA_SYSCALL_FS_BLOCK_LIST,
        (uintptr_t)devices, (uintptr_t)capacity, (uintptr_t)out_count, 0, 0, 0));
}

int terra_fs_defrag_volume_info(const char *path,
                                terra_defrag_volume_info_t *out_info) {
    if (!path || !out_info)
        return TERRA_STATUS_INVAL;
    return result_to_status(terra_syscall(TERRA_SYSCALL_FS_DEFRAG_VOLUME_INFO,
        (uintptr_t)path, (uintptr_t)out_info, 0, 0, 0, 0));
}

int terra_fs_defrag_file_page(const char *path, uint64_t generation,
                              uint64_t cursor,
                              terra_defrag_file_record_t *records,
                              size_t capacity,
                              terra_fs_query_page_t *out_page) {
    if (!path || !out_page || (capacity != 0 && !records))
        return TERRA_STATUS_INVAL;
    return result_to_status(terra_syscall(TERRA_SYSCALL_FS_DEFRAG_FILE_PAGE,
        (uintptr_t)path, (uintptr_t)generation, (uintptr_t)cursor,
        (uintptr_t)records, (uintptr_t)capacity, (uintptr_t)out_page));
}

int terra_fs_defrag_extent_page(const char *path, uint64_t generation,
                                uint64_t cursor,
                                terra_defrag_extent_record_t *records,
                                size_t capacity,
                                terra_fs_query_page_t *out_page) {
    if (!path || !out_page || (capacity != 0 && !records))
        return TERRA_STATUS_INVAL;
    return result_to_status(terra_syscall(TERRA_SYSCALL_FS_DEFRAG_EXTENT_PAGE,
        (uintptr_t)path, (uintptr_t)generation, (uintptr_t)cursor,
        (uintptr_t)records, (uintptr_t)capacity, (uintptr_t)out_page));
}

int terra_fs_defrag_free_range_page(const char *path, uint64_t generation,
                                    uint64_t cursor,
                                    terra_defrag_free_range_record_t *ranges,
                                    size_t capacity,
                                    terra_fs_query_page_t *out_page) {
    if (!path || !out_page || (capacity != 0 && !ranges))
        return TERRA_STATUS_INVAL;
    return result_to_status(terra_syscall(TERRA_SYSCALL_FS_DEFRAG_FREE_PAGE,
        (uintptr_t)path, (uintptr_t)generation, (uintptr_t)cursor,
        (uintptr_t)ranges, (uintptr_t)capacity, (uintptr_t)out_page));
}
