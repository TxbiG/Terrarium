#ifndef TERRARIUM_LIB_ABI_FS_QUERY_H
#define TERRARIUM_LIB_ABI_FS_QUERY_H

/*
 * Stable, read-only VFS/volume query ABI shared by Terrarium kernel and
 * system libraries. This ABI does not expose raw block writes and does not
 * imply that any filesystem driver can currently enumerate extents.
 */
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define TERRA_FS_QUERY_ABI_VERSION 1u
#define TERRA_FS_QUERY_PATH_MAX 128u
#define TERRA_FS_QUERY_NAME_MAX 32u
#define TERRA_FS_QUERY_MAX_MOUNTS 16u
#define TERRA_FS_QUERY_MAX_BLOCK_DEVICES 16u

/* terra_fs_mount_info_t.flags */
enum terra_fs_mount_info_flags {
    TERRA_FS_MOUNT_INFO_READONLY          = 1u << 0,
    TERRA_FS_MOUNT_INFO_PSEUDO            = 1u << 1,
    TERRA_FS_MOUNT_INFO_DISK              = 1u << 2,
    TERRA_FS_MOUNT_INFO_HAS_BLOCK_DEVICE  = 1u << 3,
    TERRA_FS_MOUNT_INFO_BLOCK_READONLY    = 1u << 4,
    TERRA_FS_MOUNT_INFO_EXTENT_QUERY      = 1u << 5,
    TERRA_FS_MOUNT_INFO_FREE_RANGE_QUERY  = 1u << 6,
    TERRA_FS_MOUNT_INFO_FILE_QUERY        = 1u << 7,
    TERRA_FS_MOUNT_INFO_VOLUME_QUERY      = 1u << 8
};

/* terra_fs_block_device_info_t.flags */
enum terra_fs_block_device_info_flags {
    TERRA_FS_BLOCK_INFO_READONLY          = 1u << 0,
    TERRA_FS_BLOCK_INFO_BYTE_SIZE_OVERFLOW= 1u << 1
};

/* terra_defrag_volume_info_t.flags: only authoritative, read-only claims. */
enum terra_defrag_snapshot_flags {
    TERRA_DEFRAG_SNAPSHOT_STABLE              = 1u << 0,
    TERRA_DEFRAG_SNAPSHOT_EXTENTS_AUTHORITATIVE= 1u << 1,
    TERRA_DEFRAG_SNAPSHOT_FREE_AUTHORITATIVE   = 1u << 2,
    TERRA_DEFRAG_SNAPSHOT_FILES_AUTHORITATIVE  = 1u << 3
};

/* Shared file flags; values intentionally match the defrag utility flags. */
enum terra_defrag_file_flags {
    TERRA_DEFRAG_FILE_REGULAR        = 1u << 0,
    TERRA_DEFRAG_FILE_SPARSE         = 1u << 1,
    TERRA_DEFRAG_FILE_SHARED         = 1u << 2,
    TERRA_DEFRAG_FILE_PINNED         = 1u << 3,
    TERRA_DEFRAG_FILE_SYSTEM         = 1u << 4,
    TERRA_DEFRAG_FILE_UNWRITABLE     = 1u << 5,
    TERRA_DEFRAG_FILE_OPEN_FOR_WRITE = 1u << 6
};

/* Mount metadata, copied out of the kernel; strings are always NUL-terminated. */
typedef struct terra_fs_mount_info {
    char source[TERRA_FS_QUERY_NAME_MAX];
    char target[TERRA_FS_QUERY_PATH_MAX];
    char type[TERRA_FS_QUERY_NAME_MAX];
    uint32_t mount_slot;          /* Opaque kernel slot; valid only with generation. */
    uint32_t fs_type_id;
    uint32_t block_device_id;     /* 0 for pseudo filesystems. */
    uint32_t flags;
    uint64_t mount_generation;    /* Changes when a mount slot is reused. */
    uint32_t sector_size_bytes;   /* Device sector size, not filesystem allocation unit. */
    uint32_t reserved;
    uint64_t sector_count;
} terra_fs_mount_info_t;

typedef struct terra_fs_block_device_info {
    uint32_t id;
    uint32_t sector_size_bytes;
    uint64_t sector_count;
    uint64_t total_bytes;         /* 0 and BYTE_SIZE_OVERFLOW on multiplication overflow. */
    uint32_t flags;
    uint32_t reserved;
    char name[TERRA_FS_QUERY_NAME_MAX];
} terra_fs_block_device_info_t;

typedef struct terra_defrag_volume_info {
    uint32_t abi_version;
    uint32_t flags;
    uint64_t volume_id;           /* Stable filesystem/provider-defined identifier. */
    uint64_t generation;          /* Pins all pages to one immutable snapshot. */
    uint32_t allocation_unit_bytes;
    uint32_t reserved;
    uint64_t allocation_unit_count;
    uint64_t file_count;
    uint64_t extent_count;
    uint64_t free_range_count;
} terra_defrag_volume_info_t;

/* File pages are ordered by increasing file_id and include zero-length files. */
typedef struct terra_defrag_file_record {
    uint64_t file_id;
    uint64_t file_generation;      /* 0 only when backend cannot provide one. */
    uint64_t logical_block_count;  /* Logical address-space span, not allocated blocks. */
    uint64_t extent_count;
    uint32_t file_flags;
    uint32_t reserved;
} terra_defrag_file_record_t;

/* Extent pages are ordered by file_id then logical_start; file_id must be nonzero. */
typedef struct terra_defrag_extent_record {
    uint64_t file_id;
    uint64_t file_generation;
    uint64_t file_block_count;     /* Logical address-space span for this file. */
    uint64_t logical_start;
    uint64_t physical_start;
    uint64_t block_count;
    uint32_t file_flags;          /* Backend maps to defrag_file_map flags. */
    uint32_t reserved;
} terra_defrag_extent_record_t;

/* Free-range pages are ordered by increasing physical_start and may not overlap. */
typedef struct terra_defrag_free_range_record {
    uint64_t physical_start;
    uint64_t block_count;
} terra_defrag_free_range_record_t;

typedef struct terra_fs_query_page {
    uint32_t count;
    uint32_t has_more;
    uint64_t next_cursor;         /* Opaque to caller; must advance when has_more is set. */
} terra_fs_query_page_t;

#ifdef __cplusplus
}
#endif
#endif
