#ifndef TERRARIUM_SYSTEM_FS_DEFRAG_H
#define TERRARIUM_SYSTEM_FS_DEFRAG_H

#include <stddef.h>
#include <stdint.h>
#include "../../lib/abi/fs_query.h"

#ifdef __cplusplus
extern "C" {
#endif

/* User/system-library wrappers for read-only kernel metadata queries. All
 * functions return TERRA_STATUS_* values except list operations: out_count
 * receives the available/required record count on success/NOSPC. */
int terra_fs_mount_info_list(terra_fs_mount_info_t *entries,
                             size_t capacity, uint32_t *out_count);
int terra_fs_resolve_mount_info(const char *path,
                                terra_fs_mount_info_t *out_info);
int terra_fs_resolve_volume_mount_info(uint64_t volume_id,
                                       terra_fs_mount_info_t *out_info);
int terra_fs_block_device_list(terra_fs_block_device_info_t *devices,
                               size_t capacity, uint32_t *out_count);
int terra_fs_defrag_volume_info(const char *path,
                                terra_defrag_volume_info_t *out_info);
int terra_fs_defrag_file_page(const char *path, uint64_t generation,
                              uint64_t cursor,
                              terra_defrag_file_record_t *records,
                              size_t capacity,
                              terra_fs_query_page_t *out_page);
int terra_fs_defrag_extent_page(const char *path, uint64_t generation,
                                uint64_t cursor,
                                terra_defrag_extent_record_t *records,
                                size_t capacity,
                                terra_fs_query_page_t *out_page);
int terra_fs_defrag_free_range_page(const char *path, uint64_t generation,
                                    uint64_t cursor,
                                    terra_defrag_free_range_record_t *ranges,
                                    size_t capacity,
                                    terra_fs_query_page_t *out_page);

#ifdef __cplusplus
}
#endif
#endif
