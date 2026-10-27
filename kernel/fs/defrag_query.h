#ifndef TERRARIUM_KERNEL_FS_DEFRAG_QUERY_H
#define TERRARIUM_KERNEL_FS_DEFRAG_QUERY_H

#include "fs.h"
#include "../../lib/abi/fs_query.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Optional, read-only provider hooks appended to terra_diskfs_driver_t.
 * The callback must pin `generation` and return only records from that exact
 * immutable snapshot. A driver without a hook is explicitly unsupported. */
typedef int (*terra_diskfs_defrag_volume_info_fn)(uint32_t block_device, const void *context, terra_defrag_volume_info_t *out_info);
typedef int (*terra_diskfs_defrag_file_page_fn)(uint32_t block_device, const void *context, uint64_t generation, uint64_t cursor, terra_defrag_file_record_t *records, size_t capacity, terra_fs_query_page_t *out_page);
typedef int (*terra_diskfs_defrag_extent_page_fn)(uint32_t block_device, const void *context, uint64_t generation, uint64_t cursor, terra_defrag_extent_record_t *records, size_t capacity, terra_fs_query_page_t *out_page);

typedef int (*terra_diskfs_defrag_free_page_fn)(
    uint32_t block_device, const void *context, uint64_t generation,
    uint64_t cursor, terra_defrag_free_range_record_t *ranges,
    size_t capacity, terra_fs_query_page_t *out_page);

/* Kernel-side VFS/volume queries. List calls are all-or-nothing: on NOSPC,
 * out_count receives the required count and no partial output is promised. */
int terra_vfs_mount_list_info(terra_fs_mount_info_t *entries, size_t capacity, uint32_t *out_count);
int terra_vfs_resolve_mount_info(const char *path, terra_fs_mount_info_t *out_info);
int terra_vfs_resolve_volume_mount_info(uint64_t volume_id, terra_fs_mount_info_t *out_info);
int terra_vfs_defrag_volume_info(const char *path, terra_defrag_volume_info_t *out_info);
int terra_vfs_defrag_file_page(const char *path, uint64_t generation, uint64_t cursor, terra_defrag_file_record_t *records, size_t capacity, terra_fs_query_page_t *out_page);
int terra_vfs_defrag_extent_page(const char *path, uint64_t generation, uint64_t cursor, terra_defrag_extent_record_t *records, size_t capacity, terra_fs_query_page_t *out_page);
int terra_vfs_defrag_free_range_page(const char *path, uint64_t generation, uint64_t cursor, terra_defrag_free_range_record_t *ranges, size_t capacity, terra_fs_query_page_t *out_page);

#ifdef __cplusplus
}
#endif
#endif
