/*
 * TerrariumOS defrag utility - single C translation unit.
 *
 * Compile-time filesystem selection:
 *   - A direct filesystem build selects exactly one of FAT32, EXFAT, EXT2, EXT4, NTFS3 or XFS.
 *   - A VFS build defines TERRARIUM_FS_VFS and may additionally define any combination of the on-disk filesystem macros it can dispatch to.
 *   - TERRARIUM_FS_EXTFAT is accepted as an alias for TERRARIUM_FS_EXFAT.
 *   - With no selection macro, a generic, non-mutating build is produced.
 * These macros identify the intended filesystem integration; they do NOT
 * claim that a filesystem backend supports extent enumeration or relocation.
 * This source deliberately refuses disk modifications until a backend provides
 * transactional move, flush, verification and recovery operations.
 */

/*
 * TODO: Complete TerrariumOS Defragmentation Utility
 *
 * STATUS:
 *   The current implementation provides compile-time filesystem selection,
 *   file-extent analysis, a relocation planner, layout validation and
 *   journal/recovery state helpers.
 *
 *   REAL DISK ANALYSIS AND RELOCATION ARE NOT IMPLEMENTED.
 *   Filesystem macros select a build configuration; they do not prove that
 *   a filesystem supports safe defragmentation.
 *
 * ---------------------------------------------------------------------------
 * 1. VFS AND VOLUME INTEGRATION
 * ---------------------------------------------------------------------------
 * [~] Integrate with the actual TerrariumOS VFS and block-device APIs.
 * [~] Implement drive-letter, volume-ID and mount-point resolution.
 * [~] Query the mounted filesystem type at runtime.
 * [~] Add a capability interface for extent queries, free-space enumeration, safe relocation, flushing, verification and recovery.
 * [~] Implement runtime dispatch to the selected filesystem backend.
 * [~] Handle unmounted, busy, read-only, encrypted and unsupported volumes.
 * [~] Ensure filesystem block numbers are never confused with bytes or physical device-sector addresses.
 *
 * ---------------------------------------------------------------------------
 * 2. FILESYSTEM BACKENDS
 * ---------------------------------------------------------------------------
 * [ ] FAT32: enumerate cluster chains and free clusters; safely update FAT entries, directory metadata and FSInfo where applicable.
 * [ ] exFAT: enumerate file allocation extents; safely update the allocation bitmap, FAT chains and directory-entry metadata as applicable.
 * [ ] ext2: support block-map and indirect-block updates, allocation bitmaps and filesystem metadata consistency.
 * [ ] ext4: support extent trees, allocation metadata and journal-aware transactions; handle unsupported features by refusing relocation.
 * [ ] NTFS3: support file runlists, $Bitmap, MFT metadata and the required NTFS consistency and recovery mechanisms.
 * [ ] XFS: support extent/BMBT updates, allocation groups and XFS transaction logging/recovery.
 * [~] VFS: dispatch to the actual mounted filesystem; do not treat VFS as an on-disk filesystem or defragmentation backend.
 * [~] Verify each filesystem's on-disk format, feature flags, mount state and supported relocation operations before enabling writes.
 *
 * NOTE: Do not implement filesystem metadata changes using generic raw-block
 * writes. Each backend must use its own correct allocation and transaction
 * mechanisms. Unsupported filesystem features must fail closed.
 *
 * ---------------------------------------------------------------------------
 * 3. ANALYSIS AND RELOCATION PLANNING
 * ---------------------------------------------------------------------------
 * [ ] Obtain authoritative file-extent and free-space snapshots from the filesystem, including allocation-unit sizes and volume boundaries.
 * [~] Correctly account for sparse files, shared/reflinked extents, hard links, compressed files, extents with special semantics and metadata files.
 * [~] Exclude pinned, unwritable, concurrently modified and unsupported files.
 * [~] Validate all allocation maps, integer arithmetic and physical ranges.
 * [~] Prevent overlapping moves, destination reuse and source/destination conflicts across the entire relocation plan.
 * [ ] Revalidate the file generation, layout and destination immediately
 *     before executing every move.
 * [~] Improve placement strategies to reduce fragmentation while preserving useful free-space regions and respecting filesystem allocation rules.
 * [~] Report fragmentation accurately; distinguish logical file extents from physical device layout and filesystem allocation units.
 *
 * ---------------------------------------------------------------------------
 * 4. SAFE RELOCATION AND CRASH RECOVERY
 * ---------------------------------------------------------------------------
 * [ ] Implement an actual persistent on-disk or filesystem-managed journal.
 * [~] Define a versioned, portable journal encoding and record integrity checks.
 * [ ] Implement durable write ordering, flush/barrier handling and atomic metadata updates appropriate to each backend and storage device.
 * [ ] Reserve destination space without exposing it to other allocations.
 * [ ] Copy data safely, including required handling of concurrent changes.
 * [ ] Verify destination contents before committing the new file mapping.
 * [ ] Commit new metadata before releasing old extents.
 * [ ] Recover deterministically after interruption at every transaction stage.
 * [~] Handle corrupt, incomplete, stale and incompatible recovery records without guessing or discarding the only valid copy of file data.
 * [ ] Add rollback where safe and explicitly define when roll-forward is required.
 * [ ] Ensure recovery itself is idempotent and safe to rerun.
 *
 * IMPORTANT: The current journal state helpers are not a persistent journal
 * implementation and do not, by themselves, guarantee crash consistency.
 *
 * ---------------------------------------------------------------------------
 * 5. SYSTEM FILES AND STORAGE TYPES
 * ---------------------------------------------------------------------------
 * [ ] Support protected/system files only through approved filesystem paths.
 * [ ] Add offline or boot-time maintenance for files that cannot safely move while open, mapped, executing or otherwise in use.
 * [ ] Coordinate with bootloader, kernel-image and recovery-partition rules.
 * [ ] Detect SSDs and virtual/block devices where possible; use TRIM/discard only when supported and keep it separate from file relocation.
 * [ ] Do not run HDD-style defragmentation automatically on SSDs without a justified filesystem-specific policy.
 * [ ] Leave filesystem journals, allocation metadata and device metadata under their respective subsystem's control.
 *
 * ---------------------------------------------------------------------------
 * 6. COMMAND-LINE INTERFACE
 * ---------------------------------------------------------------------------
 * [~] Implement real target resolution for commands such as:
 *       defrag C:
 *       defrag C: --analyse
 *       defrag C: --verbose
 *       defrag C: --status
 *       defrag --list-filesystems
 * [~] Make analysis mode strictly read-only.
 * [ ] Add progress reporting, cancellation and meaningful exit/error codes.
 * [~] Report per-filesystem capabilities honestly; distinguish analysis,
 *     planning, partial completion and successful relocation.
 * [ ] Require appropriate permissions and explicit policy for destructive work.
 * [~] Ensure unsupported or unimplemented operations never report success.
 *
 * ---------------------------------------------------------------------------
 * 7. BUILD CONFIGURATION AND MACROS
 * ---------------------------------------------------------------------------
 * [~] Keep TERRARIUM_FS_FAT32, TERRARIUM_FS_EXFAT, TERRARIUM_FS_EXT2, TERRARIUM_FS_EXT4, TERRARIUM_FS_NTFS3 and TERRARIUM_FS_XFS mutually exclusive in direct-filesystem builds.
 * [~] Keep TERRARIUM_FS_EXTFAT as a documented alias for TERRARIUM_FS_EXFAT.
 * [~] Allow TERRARIUM_FS_VFS to build multiple available backend integrations.
 * [~] Ensure the generic build stays non-mutating.
 * [~] Verify that every selected backend actually implements its advertised operations; a defined macro alone must never enable disk writes.
 * [ ] Integrate the single-file utility with the real TerrariumOS build system.
 *
 * ---------------------------------------------------------------------------
 * 8. TESTING AND VALIDATION
 * ---------------------------------------------------------------------------
 * [~] Add unit tests for analysis, layout validation, planning and journal logic.
 * [~] Test empty files, single extents, many extents, sparse files, shared extents, duplicate IDs, overlapping allocations and integer overflows.
 * [~] Test insufficient free space, read-only volumes and unsupported features.
 * [ ] Add virtual-disk integration tests for each supported filesystem.
 * [ ] Inject failures and simulated power loss after every relocation stage.
 * [ ] Verify filesystem integrity and file contents before and after recovery.
 * [~] Run sanitizers, static analysis, fuzzing and strict compiler warnings.
 * [ ] Test on disposable disk images before enabling real-device writes.
 * [ ] Verify actual TerrariumOS builds; mock builds alone are insufficient.
 *
 * ACCEPTANCE CRITERIA:
 *   A filesystem is not considered supported for defragmentation until its
 *   backend passes the required integration, integrity and recovery tests.
 *   Analysis support and relocation support must be reported separately.
 *   No operation may modify a volume unless all required safety checks pass.
 */

/* Auto-enable the public mount-list adapter when built in the Terrarium tree.
 * The current system/fs/mount.c implementation returns TERRA_STATUS_NOSYS,
 * so this path deliberately reports that limitation instead of guessing. */
#if defined(TERRARIUM_DEFRAG_USE_TERRARIUM_API)
#  include "../../system/fs/mount.h"
#  include "../../system/fs/defrag.h"
#  include "../../system/status.h"
#  define DEFRAG_HAS_TERRARIUM_MOUNT_API 1
#elif defined(__has_include)
#  if __has_include("../../system/fs/mount.h") && __has_include("../../system/fs/defrag.h") && __has_include("../../system/status.h")
#    include "../../system/fs/mount.h"
#    include "../../system/fs/defrag.h"
#    include "../../system/status.h"
#    define DEFRAG_HAS_TERRARIUM_MOUNT_API 1
#  endif
#endif
#ifndef DEFRAG_HAS_TERRARIUM_MOUNT_API
#  define DEFRAG_HAS_TERRARIUM_MOUNT_API 0
#endif

/* In the Terrarium tree, default to runtime VFS dispatch if the caller did not
 * request a generic or direct-filesystem build explicitly. */
#if DEFRAG_HAS_TERRARIUM_MOUNT_API && !defined(TERRARIUM_FS_GENERIC) && !defined(TERRARIUM_FS_VFS) && !defined(TERRARIUM_FS_FAT32) && !defined(TERRARIUM_FS_EXFAT) && !defined(TERRARIUM_FS_EXTFAT) && !defined(TERRARIUM_FS_EXT2) && !defined(TERRARIUM_FS_EXT4) && !defined(TERRARIUM_FS_NTFS3) && !defined(TERRARIUM_FS_XFS)
#  define TERRARIUM_FS_VFS 1
#endif

#if defined(TERRARIUM_FS_EXTFAT) && defined(TERRARIUM_FS_EXFAT)
#  error "Define only TERRARIUM_FS_EXFAT (or its alias TERRARIUM_FS_EXTFAT), not both"
#elif defined(TERRARIUM_FS_EXTFAT)
#  define TERRARIUM_FS_EXFAT 1
#endif

#if !defined(TERRARIUM_FS_VFS) && (defined(TERRARIUM_FS_FAT32) + defined(TERRARIUM_FS_EXFAT) + defined(TERRARIUM_FS_EXT2) + defined(TERRARIUM_FS_EXT4) + defined(TERRARIUM_FS_NTFS3) + defined(TERRARIUM_FS_XFS)) > 1
#  error "A direct filesystem build must select only one TERRARIUM_FS_* backend; use TERRARIUM_FS_VFS for multi-filesystem dispatch"
#endif

#if !defined(TERRARIUM_FS_VFS) && !defined(TERRARIUM_FS_FAT32) && !defined(TERRARIUM_FS_EXFAT) && !defined(TERRARIUM_FS_EXT2) && !defined(TERRARIUM_FS_EXT4) && !defined(TERRARIUM_FS_NTFS3) && !defined(TERRARIUM_FS_XFS)
#  if !defined(TERRARIUM_FS_GENERIC)
#    define TERRARIUM_FS_GENERIC 1
#  endif
#endif

#if defined(TERRARIUM_FS_GENERIC) && (defined(TERRARIUM_FS_VFS) || defined(TERRARIUM_FS_FAT32) || defined(TERRARIUM_FS_EXFAT) || defined(TERRARIUM_FS_EXT2) || defined(TERRARIUM_FS_EXT4) || defined(TERRARIUM_FS_NTFS3) || defined(TERRARIUM_FS_XFS))
#  error "TERRARIUM_FS_GENERIC cannot be combined with a specific TERRARIUM_FS_* selection"
#endif

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define DEFRAG_JOURNAL_MAGIC UINT32_C(0x4a464454) /* bytes "TDFJ" little-endian */
#define DEFRAG_JOURNAL_VERSION 1u

typedef enum defrag_txn_state {
    DEFRAG_TXN_EMPTY = 0,
    DEFRAG_TXN_PREPARED = 1,
    DEFRAG_TXN_DEST_RESERVED = 2,
    DEFRAG_TXN_DATA_COPIED = 3,
    DEFRAG_TXN_DATA_VERIFIED = 4,
    DEFRAG_TXN_METADATA_COMMITTED = 5,
    DEFRAG_TXN_SOURCE_RELEASED = 6,
    DEFRAG_TXN_COMPLETE = 7,
    DEFRAG_TXN_ABORTED = 8
} defrag_txn_state_t;

typedef struct defrag_journal_record {
    uint32_t magic;
    uint16_t version;
    uint16_t state;
    uint64_t sequence;
    uint64_t file_id;
    uint64_t expected_generation;
    uint64_t destination_start;
    uint64_t block_count;
    uint64_t source_layout_hash;
    uint32_t crc32;
} defrag_journal_record_t;

typedef enum defrag_recovery_action {
    DEFRAG_RECOVERY_INVALID_RECORD = 0,
    DEFRAG_RECOVERY_CLEAR_KEEP_SOURCE,
    DEFRAG_RECOVERY_DISCARD_DEST_KEEP_SOURCE,
    DEFRAG_RECOVERY_VERIFY_DESTINATION,
    DEFRAG_RECOVERY_COMMIT_NEW_MAPPING,
    DEFRAG_RECOVERY_RELEASE_OLD_EXTENTS,
    DEFRAG_RECOVERY_MARK_COMPLETE,
    DEFRAG_RECOVERY_CLEAR_COMPLETED_RECORD
} defrag_recovery_action_t;

typedef enum defrag_fs_kind {
    DEFRAG_FS_UNKNOWN = 0,
    DEFRAG_FS_VFS,
    DEFRAG_FS_FAT32,
    DEFRAG_FS_EXFAT,
    DEFRAG_FS_EXT2,
    DEFRAG_FS_EXT4,
    DEFRAG_FS_NTFS3,
    DEFRAG_FS_XFS
} defrag_fs_kind_t;

typedef struct defrag_fs_profile {
    const char *name;
    defrag_fs_kind_t kind;
    int selected_for_build;
    int probe_code_present;
    int mount_descriptor_present;
    int currently_registered_readonly;
    int extent_enumeration_available;
    int safe_relocation_available;
    const char *detail;
} defrag_fs_profile_t;

const defrag_fs_profile_t *defrag_fs_profiles(size_t *count);
const defrag_fs_profile_t *defrag_fs_profile_by_name(const char *name);

#if defined(TERRARIUM_FS_VFS)
#  define DEFRAG_BUILD_FS_KIND DEFRAG_FS_VFS
#  define DEFRAG_BUILD_FS_NAME "vfs dispatch"
#  define DEFRAG_PROFILE_VFS_SELECTED 1
#else
#  define DEFRAG_PROFILE_VFS_SELECTED 0
#endif

#if defined(TERRARIUM_FS_FAT32)
#  define DEFRAG_PROFILE_FAT32_SELECTED 1
#else
#  define DEFRAG_PROFILE_FAT32_SELECTED 0
#endif
#if defined(TERRARIUM_FS_EXFAT)
#  define DEFRAG_PROFILE_EXFAT_SELECTED 1
#else
#  define DEFRAG_PROFILE_EXFAT_SELECTED 0
#endif
#if defined(TERRARIUM_FS_EXT2)
#  define DEFRAG_PROFILE_EXT2_SELECTED 1
#else
#  define DEFRAG_PROFILE_EXT2_SELECTED 0
#endif
#if defined(TERRARIUM_FS_EXT4)
#  define DEFRAG_PROFILE_EXT4_SELECTED 1
#else
#  define DEFRAG_PROFILE_EXT4_SELECTED 0
#endif
#if defined(TERRARIUM_FS_NTFS3)
#  define DEFRAG_PROFILE_NTFS3_SELECTED 1
#else
#  define DEFRAG_PROFILE_NTFS3_SELECTED 0
#endif
#if defined(TERRARIUM_FS_XFS)
#  define DEFRAG_PROFILE_XFS_SELECTED 1
#else
#  define DEFRAG_PROFILE_XFS_SELECTED 0
#endif

#if !defined(TERRARIUM_FS_VFS)
#  if defined(TERRARIUM_FS_FAT32)
#    define DEFRAG_BUILD_FS_KIND DEFRAG_FS_FAT32
#    define DEFRAG_BUILD_FS_NAME "fat32"
#  elif defined(TERRARIUM_FS_EXFAT)
#    define DEFRAG_BUILD_FS_KIND DEFRAG_FS_EXFAT
#    define DEFRAG_BUILD_FS_NAME "exfat"
#  elif defined(TERRARIUM_FS_EXT2)
#    define DEFRAG_BUILD_FS_KIND DEFRAG_FS_EXT2
#    define DEFRAG_BUILD_FS_NAME "ext2"
#  elif defined(TERRARIUM_FS_EXT4)
#    define DEFRAG_BUILD_FS_KIND DEFRAG_FS_EXT4
#    define DEFRAG_BUILD_FS_NAME "ext4"
#  elif defined(TERRARIUM_FS_NTFS3)
#    define DEFRAG_BUILD_FS_KIND DEFRAG_FS_NTFS3
#    define DEFRAG_BUILD_FS_NAME "ntfs3"
#  elif defined(TERRARIUM_FS_XFS)
#    define DEFRAG_BUILD_FS_KIND DEFRAG_FS_XFS
#    define DEFRAG_BUILD_FS_NAME "xfs"
#  else
#    define DEFRAG_BUILD_FS_KIND DEFRAG_FS_UNKNOWN
#    define DEFRAG_BUILD_FS_NAME "generic (no filesystem backend selected)"
#  endif
#endif

defrag_fs_kind_t defrag_build_fs_kind(void) { return DEFRAG_BUILD_FS_KIND; }

const char *defrag_build_fs_name(void) { return DEFRAG_BUILD_FS_NAME; }

enum {
    DEFRAG_FILE_REGULAR       = 1u << 0,
    DEFRAG_FILE_SPARSE        = 1u << 1,
    DEFRAG_FILE_SHARED        = 1u << 2, /* reflink/shared extents or hard-link ambiguity */
    DEFRAG_FILE_PINNED        = 1u << 3,
    DEFRAG_FILE_SYSTEM        = 1u << 4,
    DEFRAG_FILE_UNWRITABLE    = 1u << 5,
    DEFRAG_FILE_OPEN_FOR_WRITE= 1u << 6
};

typedef struct defrag_extent {
    uint64_t logical_start;  /* filesystem allocation units, not bytes */
    uint64_t physical_start; /* filesystem allocation units, not bytes */
    uint64_t block_count;
} defrag_extent_t;

typedef struct defrag_file_map {
    uint64_t file_id;              /* stable backend-specific file/inode identifier */
    uint64_t generation;           /* backend generation/version; zero if unavailable */
    uint64_t block_count;          /* logical allocation units occupied by the file */
    uint32_t flags;
    const defrag_extent_t *extents;/* sorted by logical_start */
    size_t extent_count;
} defrag_file_map_t;

typedef struct defrag_free_range {
    uint64_t physical_start;
    uint64_t block_count;
} defrag_free_range_t;

typedef enum defrag_skip_reason {
    DEFRAG_SKIP_NONE = 0,
    DEFRAG_SKIP_NOT_REGULAR,
    DEFRAG_SKIP_SPARSE_OR_GAPPED,
    DEFRAG_SKIP_SHARED,
    DEFRAG_SKIP_PINNED,
    DEFRAG_SKIP_SYSTEM,
    DEFRAG_SKIP_UNWRITABLE,
    DEFRAG_SKIP_OPEN_FOR_WRITE,
    DEFRAG_SKIP_NOT_FRAGMENTED,
    DEFRAG_SKIP_NO_CONTIGUOUS_SPACE
} defrag_skip_reason_t;

typedef struct defrag_file_analysis {
    uint64_t file_id;
    uint64_t generation;
    uint64_t block_count;
    size_t extent_count;
    size_t physical_fragments;
    int fragmented;
    int eligible;
    defrag_skip_reason_t skip_reason;
} defrag_file_analysis_t;

typedef enum defrag_result {
    DEFRAG_RESULT_OK = 0,
    DEFRAG_RESULT_INVALID_ARGUMENT = -1,
    DEFRAG_RESULT_CORRUPT_MAP = -2,
    DEFRAG_RESULT_OUT_OF_MEMORY = -3,
    DEFRAG_RESULT_OVERFLOW = -4,
    DEFRAG_RESULT_UNSUPPORTED_FS = -5,
    DEFRAG_RESULT_UNSAFE_VOLUME = -6,
    DEFRAG_RESULT_NO_AUTHORITATIVE_SNAPSHOT = -7,
    DEFRAG_RESULT_READ_ONLY = -8
} defrag_result_t;

typedef struct defrag_plan_move {
    uint64_t file_id;
    uint64_t expected_generation;
    uint64_t expected_layout_hash;
    uint64_t destination_start;
    uint64_t block_count;
    size_t source_extent_count;
} defrag_plan_move_t;

typedef struct defrag_plan_stats {
    size_t files_scanned;
    size_t files_fragmented;
    size_t files_skipped;
    size_t moves_planned;
    size_t files_without_contiguous_space;
} defrag_plan_stats_t;

typedef struct defrag_plan {
    defrag_plan_move_t *moves;
    size_t move_count;
    defrag_plan_stats_t stats;
} defrag_plan_t;

enum {
    DEFRAG_VOLUME_READ_ONLY               = 1u << 0,
    DEFRAG_VOLUME_BUSY                    = 1u << 1,
    DEFRAG_VOLUME_ENCRYPTION_OPAQUE        = 1u << 2,
    DEFRAG_VOLUME_UNSUPPORTED_FEATURES     = 1u << 3,
    DEFRAG_VOLUME_SNAPSHOT_STABLE          = 1u << 4,
    DEFRAG_VOLUME_EXTENTS_AUTHORITATIVE    = 1u << 5,
    DEFRAG_VOLUME_RELOCATION_CAPABLE       = 1u << 6,
    DEFRAG_VOLUME_FILES_AUTHORITATIVE      = 1u << 7,
    DEFRAG_VOLUME_FREE_AUTHORITATIVE       = 1u << 8
};

typedef struct defrag_volume_snapshot {
    uint64_t volume_id;                /* stable backend/VFS volume identifier */
    uint64_t generation;               /* nonzero snapshot generation */
    defrag_fs_kind_t filesystem;       /* must be an on-disk filesystem kind */
    uint32_t flags;
    uint32_t allocation_unit_bytes;    /* filesystem allocation unit, not device sector */
    uint64_t allocation_unit_count;    /* total addressable allocation units */
    const defrag_file_map_t *files;
    size_t file_count;
    const defrag_free_range_t *free_ranges;
    size_t free_range_count;
} defrag_volume_snapshot_t;

typedef struct defrag_volume_analysis {
    uint64_t volume_id;
    uint64_t generation;
    uint64_t allocation_unit_count;
    uint32_t allocation_unit_bytes;
    size_t files_scanned;
    size_t files_fragmented;
    size_t files_skipped;
    size_t extents_scanned;
    size_t moves_plannable;
    size_t files_without_contiguous_space;
} defrag_volume_analysis_t;

/*
 * These entries describe the current TerrariumOS source, not aspirational
 * support. diskfs.c has FAT32/ext2 probes but only registers mount/describe
 * drivers for exFAT/ext4/NTFS3/XFS. All six disk types are read-only registered.
 * None exports file extents, free-range enumeration or transactional relocation.
 */
static const defrag_fs_profile_t g_profiles[] = {
    { "vfs",   DEFRAG_FS_VFS,   DEFRAG_PROFILE_VFS_SELECTED,   0, 0, 0, 0, 0,
      "VFS is a routing layer, not an on-disk filesystem. Public mount-list API currently returns NOSYS; no extent/move ABI exists." },
    { "fat32", DEFRAG_FS_FAT32, DEFRAG_PROFILE_FAT32_SELECTED, 1, 0, 1, 0, 0,
      "FAT32 probe exists, but no terra_diskfs_driver is registered. FAT-chain enumeration, allocation updates and recovery are absent." },
    { "exfat", DEFRAG_FS_EXFAT, DEFRAG_PROFILE_EXFAT_SELECTED, 1, 1, 1, 0, 0,
      "Probe/mount/describe driver exists, but it is not a file-operation backend. Allocation-bitmap/FAT transactions and recovery are absent." },
    { "ext2",  DEFRAG_FS_EXT2,  DEFRAG_PROFILE_EXT2_SELECTED,  1, 0, 1, 0, 0,
      "ext2 probe exists, but no terra_diskfs_driver is registered. Block-map enumeration, allocation updates and recovery are absent." },
    { "ext4",  DEFRAG_FS_EXT4,  DEFRAG_PROFILE_EXT4_SELECTED,  1, 1, 1, 0, 0,
      "Probe/mount/describe driver exists, but extent-tree operations, allocation transactions and journal-coordinated relocation are absent." },
    { "ntfs3", DEFRAG_FS_NTFS3, DEFRAG_PROFILE_NTFS3_SELECTED, 1, 1, 1, 0, 0,
      "Probe/mount/describe driver exists, but NTFS runlist/$Bitmap/MFT transactions and file extents are not exposed." },
    { "xfs",   DEFRAG_FS_XFS,   DEFRAG_PROFILE_XFS_SELECTED,   1, 1, 1, 0, 0,
      "Probe/mount/describe driver exists, but XFS extent/BMBT updates, allocation-group transactions and recovery are not exposed." }
};

const defrag_fs_profile_t *defrag_fs_profiles(size_t *count) {
    if (count)
        *count = sizeof(g_profiles) / sizeof(g_profiles[0]);
    return g_profiles;
}

static int ascii_lower(int c) {
    if (c >= 'A' && c <= 'Z')
        return c - 'A' + 'a';
    return c;
}

static int name_equal_ci(const char *a, const char *b) {
    size_t i = 0;
    if (!a || !b)
        return 0;
    while (a[i] && b[i]) {
        if (ascii_lower((unsigned char)a[i]) != ascii_lower((unsigned char)b[i]))
            return 0;
        ++i;
    }
    return a[i] == '\0' && b[i] == '\0';
}

const defrag_fs_profile_t *defrag_fs_profile_by_name(const char *name) {
    size_t i, n;
    const defrag_fs_profile_t *profiles = defrag_fs_profiles(&n);
    for (i = 0; i < n; ++i) {
        if (name_equal_ci(name, profiles[i].name))
            return &profiles[i];
    }
    /* Accept the user's common spelling, but report the canonical exFAT name. */
    if (name_equal_ci(name, "extfat")) {
        for (i = 0; i < n; ++i) {
            if (profiles[i].kind == DEFRAG_FS_EXFAT)
                return &profiles[i];
        }
    }
    return NULL;
}

static int add_overflows_u64(uint64_t a, uint64_t b) {
    return b > UINT64_MAX - a;
}

static uint64_t hash_u64(uint64_t hash, uint64_t value) {
    unsigned i;
    /* Hash in canonical little-endian byte order independent of host ABI. */
    for (i = 0; i < 8; ++i) {
        hash ^= (uint8_t)(value & 0xffu);
        hash *= UINT64_C(1099511628211);
        value >>= 8;
    }
    return hash;
}

uint64_t defrag_layout_hash(const defrag_file_map_t *file) {
    uint64_t hash = UINT64_C(1469598103934665603);
    size_t i;
    if (!file || (!file->extents && file->extent_count != 0))
        return 0;
    hash = hash_u64(hash, file->file_id);
    hash = hash_u64(hash, file->generation);
    hash = hash_u64(hash, file->block_count);
    hash = hash_u64(hash, (uint64_t)file->extent_count);
    for (i = 0; i < file->extent_count; ++i) {
        hash = hash_u64(hash, file->extents[i].logical_start);
        hash = hash_u64(hash, file->extents[i].physical_start);
        hash = hash_u64(hash, file->extents[i].block_count);
    }
    return hash;
}

const char *defrag_skip_reason_string(defrag_skip_reason_t reason) {
    switch (reason) {
        case DEFRAG_SKIP_NONE: return "eligible";
        case DEFRAG_SKIP_NOT_REGULAR: return "not a regular file";
        case DEFRAG_SKIP_SPARSE_OR_GAPPED: return "sparse or non-contiguous logical map";
        case DEFRAG_SKIP_SHARED: return "shared/reflinked or hard-link mapping";
        case DEFRAG_SKIP_PINNED: return "pinned by filesystem or device";
        case DEFRAG_SKIP_SYSTEM: return "filesystem/system metadata";
        case DEFRAG_SKIP_UNWRITABLE: return "file or volume is read-only";
        case DEFRAG_SKIP_OPEN_FOR_WRITE: return "open for concurrent write";
        case DEFRAG_SKIP_NOT_FRAGMENTED: return "already physically contiguous";
        case DEFRAG_SKIP_NO_CONTIGUOUS_SPACE: return "no suitable contiguous free range";
        default: return "unknown reason";
    }
}

static defrag_skip_reason_t flag_skip_reason(uint32_t flags) {
    if (!(flags & DEFRAG_FILE_REGULAR)) return DEFRAG_SKIP_NOT_REGULAR;
    if (flags & DEFRAG_FILE_SPARSE) return DEFRAG_SKIP_SPARSE_OR_GAPPED;
    if (flags & DEFRAG_FILE_SHARED) return DEFRAG_SKIP_SHARED;
    if (flags & DEFRAG_FILE_PINNED) return DEFRAG_SKIP_PINNED;
    if (flags & DEFRAG_FILE_SYSTEM) return DEFRAG_SKIP_SYSTEM;
    if (flags & DEFRAG_FILE_UNWRITABLE) return DEFRAG_SKIP_UNWRITABLE;
    if (flags & DEFRAG_FILE_OPEN_FOR_WRITE) return DEFRAG_SKIP_OPEN_FOR_WRITE;
    return DEFRAG_SKIP_NONE;
}

/* Validate the logical shape of every file map, including files the planner
 * will later skip. This prevents a skipped/system/shared file from hiding a
 * malformed extent map in the global volume snapshot. */
static defrag_result_t defrag_validate_file_map_shape(const defrag_file_map_t *file) {
    size_t i;
    uint64_t previous_logical_end = 0;
    int have_previous = 0;
    int sparse;

    if (!file)
        return DEFRAG_RESULT_INVALID_ARGUMENT;
    if (!file->extents && file->extent_count != 0)
        return DEFRAG_RESULT_CORRUPT_MAP;
    sparse = (file->flags & DEFRAG_FILE_SPARSE) != 0;
    if (file->block_count == 0 && file->extent_count != 0)
        return DEFRAG_RESULT_CORRUPT_MAP;
    if (file->block_count != 0 && file->extent_count == 0 && !sparse)
        return DEFRAG_RESULT_CORRUPT_MAP;

    for (i = 0; i < file->extent_count; ++i) {
        const defrag_extent_t *e = &file->extents[i];
        uint64_t logical_end;
        if (e->block_count == 0 ||
            add_overflows_u64(e->logical_start, e->block_count) ||
            add_overflows_u64(e->physical_start, e->block_count))
            return DEFRAG_RESULT_CORRUPT_MAP;
        logical_end = e->logical_start + e->block_count;
        if (logical_end > file->block_count)
            return DEFRAG_RESULT_CORRUPT_MAP;
        if (have_previous && e->logical_start < previous_logical_end)
            return DEFRAG_RESULT_CORRUPT_MAP;
        if (!sparse && e->logical_start != previous_logical_end)
            return DEFRAG_RESULT_CORRUPT_MAP;
        previous_logical_end = logical_end;
        have_previous = 1;
    }
    if (!sparse && file->block_count != 0 && previous_logical_end != file->block_count)
        return DEFRAG_RESULT_CORRUPT_MAP;
    return DEFRAG_RESULT_OK;
}


defrag_result_t defrag_analyze_file(const defrag_file_map_t *file,
                                   defrag_file_analysis_t *out_analysis) {
    size_t i;
    uint64_t expected_logical = 0;
    uint64_t previous_physical_end = 0;
    int have_previous = 0;
    defrag_file_analysis_t result;

    if (!file || !out_analysis || (!file->extents && file->extent_count != 0))
        return DEFRAG_RESULT_INVALID_ARGUMENT;
    {
        defrag_result_t shape_status = defrag_validate_file_map_shape(file);
        if (shape_status != DEFRAG_RESULT_OK)
            return shape_status;
    }
    memset(&result, 0, sizeof(result));
    result.file_id = file->file_id;
    result.generation = file->generation;
    result.block_count = file->block_count;
    result.extent_count = file->extent_count;

    result.skip_reason = flag_skip_reason(file->flags);
    if (result.skip_reason != DEFRAG_SKIP_NONE) {
        *out_analysis = result;
        return DEFRAG_RESULT_OK;
    }
    if (file->block_count == 0) {
        if (file->extent_count != 0)
            return DEFRAG_RESULT_CORRUPT_MAP;
        result.skip_reason = DEFRAG_SKIP_NOT_FRAGMENTED;
        *out_analysis = result;
        return DEFRAG_RESULT_OK;
    }
    if (file->extent_count == 0)
        return DEFRAG_RESULT_CORRUPT_MAP;

    for (i = 0; i < file->extent_count; ++i) {
        const defrag_extent_t *extent = &file->extents[i];
        uint64_t logical_end, physical_end;
        if (extent->block_count == 0 ||
            add_overflows_u64(extent->logical_start, extent->block_count) ||
            add_overflows_u64(extent->physical_start, extent->block_count))
            return DEFRAG_RESULT_CORRUPT_MAP;
        logical_end = extent->logical_start + extent->block_count;
        physical_end = extent->physical_start + extent->block_count;

        if (extent->logical_start < expected_logical)
            return DEFRAG_RESULT_CORRUPT_MAP; /* overlap/out-of-order */
        if (extent->logical_start != expected_logical) {
            /* Structural validation permits logical gaps only for sparse files;
             * those files are not eligible for this whole-file relocation planner. */
            result.skip_reason = DEFRAG_SKIP_SPARSE_OR_GAPPED;
            result.physical_fragments = 0;
            result.fragmented = 0;
            result.eligible = 0;
            *out_analysis = result;
            return DEFRAG_RESULT_OK;
        }

        if (!have_previous || previous_physical_end != extent->physical_start)
            ++result.physical_fragments;
        expected_logical = logical_end;
        previous_physical_end = physical_end;
        have_previous = 1;
    }

    if (expected_logical != file->block_count) {
        result.skip_reason = DEFRAG_SKIP_SPARSE_OR_GAPPED;
        *out_analysis = result;
        return DEFRAG_RESULT_OK;
    }
    result.fragmented = result.physical_fragments > 1;
    result.eligible = 1;
    result.skip_reason = result.fragmented ? DEFRAG_SKIP_NONE : DEFRAG_SKIP_NOT_FRAGMENTED;
    *out_analysis = result;
    return DEFRAG_RESULT_OK;
}

typedef struct free_range_work {
    uint64_t start;
    uint64_t count;
} free_range_work_t;

typedef struct candidate_work {
    const defrag_file_map_t *file;
    defrag_file_analysis_t analysis;
} candidate_work_t;

typedef struct physical_span_work {
    uint64_t start;
    uint64_t end;
    uint64_t file_id;
    uint32_t flags;
} physical_span_work_t;

static int compare_u64(const void *left, const void *right) {
    uint64_t a = *(const uint64_t *)left;
    uint64_t b = *(const uint64_t *)right;
    return a < b ? -1 : (a > b ? 1 : 0);
}

static int compare_physical_spans(const void *left, const void *right) {
    const physical_span_work_t *a = (const physical_span_work_t *)left;
    const physical_span_work_t *b = (const physical_span_work_t *)right;
    if (a->start < b->start) return -1;
    if (a->start > b->start) return 1;
    if (a->end < b->end) return -1;
    if (a->end > b->end) return 1;
    if (a->file_id < b->file_id) return -1;
    if (a->file_id > b->file_id) return 1;
    return 0;
}

/*
 * Validate that the supplied snapshot is internally safe to plan against.
 * Free ranges may never intersect allocated extents. Physical overlap between
 * distinct files is allowed only when both maps explicitly declare sharing.
 */
static defrag_result_t validate_snapshot_allocations(
    const defrag_file_map_t *files, size_t file_count,
    const free_range_work_t *free_ranges, size_t free_range_count) {
    uint64_t *ids = NULL;
    physical_span_work_t *spans = NULL;
    size_t total_extents = 0, i, j, span_index = 0;
    defrag_result_t result = DEFRAG_RESULT_OK;

    if (file_count > SIZE_MAX / sizeof(*ids))
        return DEFRAG_RESULT_OVERFLOW;
    if (file_count) {
        ids = (uint64_t *)malloc(file_count * sizeof(*ids));
        if (!ids)
            return DEFRAG_RESULT_OUT_OF_MEMORY;
    }
    for (i = 0; i < file_count; ++i) {
        result = defrag_validate_file_map_shape(&files[i]);
        if (result != DEFRAG_RESULT_OK)
            goto done;
        if (files[i].extent_count && !files[i].extents) {
            result = DEFRAG_RESULT_CORRUPT_MAP;
            goto done;
        }
        if (files[i].extent_count > SIZE_MAX - total_extents) {
            result = DEFRAG_RESULT_OVERFLOW;
            goto done;
        }
        total_extents += files[i].extent_count;
        ids[i] = files[i].file_id;
    }
    if (file_count > 1) {
        qsort(ids, file_count, sizeof(*ids), compare_u64);
        for (i = 1; i < file_count; ++i) {
            if (ids[i] == ids[i - 1]) {
                result = DEFRAG_RESULT_CORRUPT_MAP; /* backend must deduplicate hard links */
                goto done;
            }
        }
    }
    if (total_extents > SIZE_MAX / sizeof(*spans)) {
        result = DEFRAG_RESULT_OVERFLOW;
        goto done;
    }
    if (total_extents) {
        spans = (physical_span_work_t *)malloc(total_extents * sizeof(*spans));
        if (!spans) {
            result = DEFRAG_RESULT_OUT_OF_MEMORY;
            goto done;
        }
    }
    for (i = 0; i < file_count; ++i) {
        if (files[i].extent_count != 0 && spans == NULL) {
            result = DEFRAG_RESULT_CORRUPT_MAP;
            goto done;
        }
        for (j = 0; j < files[i].extent_count; ++j) {
            const defrag_extent_t *e = &files[i].extents[j];
            if (e->block_count == 0 || add_overflows_u64(e->physical_start, e->block_count)) {
                result = DEFRAG_RESULT_CORRUPT_MAP;
                goto done;
            }
            spans[span_index].start = e->physical_start;
            spans[span_index].end = e->physical_start + e->block_count;
            spans[span_index].file_id = files[i].file_id;
            spans[span_index].flags = files[i].flags;
            ++span_index;
        }
    }
    if (total_extents > 1)
        qsort(spans, total_extents, sizeof(*spans), compare_physical_spans);
    for (i = 0; i < total_extents; ++i) {
        for (j = i + 1; j < total_extents && spans[j].start < spans[i].end; ++j) {
            if (spans[j].file_id == spans[i].file_id ||
                (spans[i].flags & DEFRAG_FILE_SHARED) == 0 ||
                (spans[j].flags & DEFRAG_FILE_SHARED) == 0) {
                result = DEFRAG_RESULT_CORRUPT_MAP;
                goto done;
            }
        }
    }
    /* Both arrays are sorted. Walk them without quadratic free-space checks. */
    j = 0;
    for (i = 0; i < free_range_count; ++i) {
        uint64_t free_end = free_ranges[i].start + free_ranges[i].count;
        while (j < total_extents && spans[j].end <= free_ranges[i].start)
            ++j;
        if (j < total_extents && spans[j].start < free_end) {
            result = DEFRAG_RESULT_CORRUPT_MAP;
            goto done;
        }
    }

done:
    free(ids);
    free(spans);
    return result;
}

static int compare_ranges(const void *left, const void *right) {
    const free_range_work_t *a = (const free_range_work_t *)left;
    const free_range_work_t *b = (const free_range_work_t *)right;
    if (a->start < b->start) return -1;
    if (a->start > b->start) return 1;
    if (a->count < b->count) return -1;
    if (a->count > b->count) return 1;
    return 0;
}

static int compare_candidates_largest_first(const void *left, const void *right) {
    const candidate_work_t *a = (const candidate_work_t *)left;
    const candidate_work_t *b = (const candidate_work_t *)right;
    if (a->file->block_count > b->file->block_count) return -1;
    if (a->file->block_count < b->file->block_count) return 1;
    if (a->file->file_id < b->file->file_id) return -1;
    if (a->file->file_id > b->file->file_id) return 1;
    return 0;
}

defrag_result_t defrag_plan_build(const defrag_file_map_t *files, size_t file_count, const defrag_free_range_t *free_ranges, size_t free_range_count, defrag_plan_t *out_plan) {
    free_range_work_t *ranges = NULL;
    candidate_work_t *candidates = NULL;
    defrag_plan_move_t *moves = NULL;
    size_t candidate_count = 0, i, j, move_count = 0;
    defrag_plan_t result;

    if (!out_plan || (!files && file_count != 0) ||
        (!free_ranges && free_range_count != 0))
        return DEFRAG_RESULT_INVALID_ARGUMENT;
    memset(&result, 0, sizeof(result));
    memset(out_plan, 0, sizeof(*out_plan));
    if (file_count > SIZE_MAX / sizeof(*candidates) ||
        free_range_count > SIZE_MAX / sizeof(*ranges) ||
        file_count > SIZE_MAX / sizeof(*moves))
        return DEFRAG_RESULT_OVERFLOW;

    if (free_range_count) {
        ranges = (free_range_work_t *)malloc(free_range_count * sizeof(*ranges));
        if (!ranges) return DEFRAG_RESULT_OUT_OF_MEMORY;
        for (i = 0; i < free_range_count; ++i) {
            if (free_ranges[i].block_count == 0 ||
                add_overflows_u64(free_ranges[i].physical_start, free_ranges[i].block_count)) {
                free(ranges);
                return DEFRAG_RESULT_CORRUPT_MAP;
            }
            ranges[i].start = free_ranges[i].physical_start;
            ranges[i].count = free_ranges[i].block_count;
        }
        if (free_range_count > 1)
            qsort(ranges, free_range_count, sizeof(*ranges), compare_ranges);
        for (i = 1; i < free_range_count; ++i) {
            uint64_t previous_end = ranges[i - 1].start + ranges[i - 1].count;
            if (ranges[i].start < previous_end) {
                free(ranges);
                return DEFRAG_RESULT_CORRUPT_MAP;
            }
        }
    }
    {
        defrag_result_t validation = validate_snapshot_allocations(files, file_count, ranges, free_range_count);
        if (validation != DEFRAG_RESULT_OK) {
            free(ranges);
            return validation;
        }
    }

    if (file_count) {
        candidates = (candidate_work_t *)malloc(file_count * sizeof(*candidates));
        moves = (defrag_plan_move_t *)calloc(file_count, sizeof(*moves));
        if (!candidates || !moves) {
            free(ranges); free(candidates); free(moves);
            return DEFRAG_RESULT_OUT_OF_MEMORY;
        }
    }

    result.stats.files_scanned = file_count;
    for (i = 0; i < file_count; ++i) {
        defrag_file_analysis_t analysis;
        defrag_result_t status = defrag_analyze_file(&files[i], &analysis);
        if (status != DEFRAG_RESULT_OK) {
            free(ranges); free(candidates); free(moves);
            return status;
        }
        if (analysis.fragmented)
            ++result.stats.files_fragmented;
        if (!analysis.eligible || !analysis.fragmented) {
            ++result.stats.files_skipped;
            continue;
        }
        candidates[candidate_count].file = &files[i];
        candidates[candidate_count].analysis = analysis;
        ++candidate_count;
    }
    if (candidate_count > 1)
        qsort(candidates, candidate_count, sizeof(*candidates), compare_candidates_largest_first);

    for (i = 0; i < candidate_count; ++i) {
        const defrag_file_map_t *file = candidates[i].file;
        size_t best = SIZE_MAX;
        uint64_t best_count = UINT64_MAX;
        for (j = 0; j < free_range_count; ++j) {
            if (ranges[j].count >= file->block_count && ranges[j].count < best_count) {
                best = j;
                best_count = ranges[j].count;
            } else if (ranges[j].count >= file->block_count && ranges[j].count == best_count &&
                       best != SIZE_MAX && ranges[j].start < ranges[best].start) {
                best = j;
            }
        }
        if (best == SIZE_MAX) {
            ++result.stats.files_without_contiguous_space;
            continue;
        }
        moves[move_count].file_id = file->file_id;
        moves[move_count].expected_generation = file->generation;
        moves[move_count].expected_layout_hash = defrag_layout_hash(file);
        moves[move_count].destination_start = ranges[best].start;
        moves[move_count].block_count = file->block_count;
        moves[move_count].source_extent_count = file->extent_count;
        ++move_count;
        ranges[best].start += file->block_count;
        ranges[best].count -= file->block_count;
    }

    free(ranges);
    free(candidates);
    if (move_count == 0) {
        free(moves);
        moves = NULL;
    } else {
        defrag_plan_move_t *shrunk = (defrag_plan_move_t *)realloc(moves, move_count * sizeof(*moves));
        if (shrunk)
            moves = shrunk;
    }
    result.moves = moves;
    result.move_count = move_count;
    result.stats.moves_planned = move_count;
    *out_plan = result;
    return DEFRAG_RESULT_OK;
}

void defrag_plan_destroy(defrag_plan_t *plan) {
    if (!plan)
        return;
    free(plan->moves);
    memset(plan, 0, sizeof(*plan));
}

static int defrag_is_disk_filesystem(defrag_fs_kind_t kind) {
    return kind == DEFRAG_FS_FAT32 || kind == DEFRAG_FS_EXFAT ||
           kind == DEFRAG_FS_EXT2 || kind == DEFRAG_FS_EXT4 ||
           kind == DEFRAG_FS_NTFS3 || kind == DEFRAG_FS_XFS;
}

static defrag_result_t defrag_validate_volume_snapshot(const defrag_volume_snapshot_t *volume) {
    size_t i, j;
    if (!volume || (!volume->files && volume->file_count != 0) ||
        (!volume->free_ranges && volume->free_range_count != 0) ||
        volume->allocation_unit_bytes == 0 || volume->allocation_unit_count == 0)
        return DEFRAG_RESULT_INVALID_ARGUMENT;
    if (!defrag_is_disk_filesystem(volume->filesystem))
        return DEFRAG_RESULT_UNSUPPORTED_FS;
#if !defined(TERRARIUM_FS_VFS) && !defined(TERRARIUM_FS_GENERIC)
    if (volume->filesystem != DEFRAG_BUILD_FS_KIND)
        return DEFRAG_RESULT_UNSUPPORTED_FS;
#endif
    if (volume->flags & (DEFRAG_VOLUME_BUSY | DEFRAG_VOLUME_ENCRYPTION_OPAQUE |
                         DEFRAG_VOLUME_UNSUPPORTED_FEATURES))
        return DEFRAG_RESULT_UNSAFE_VOLUME;
    if ((volume->flags & (DEFRAG_VOLUME_SNAPSHOT_STABLE |
                          DEFRAG_VOLUME_FILES_AUTHORITATIVE |
                          DEFRAG_VOLUME_EXTENTS_AUTHORITATIVE |
                          DEFRAG_VOLUME_FREE_AUTHORITATIVE)) !=
        (DEFRAG_VOLUME_SNAPSHOT_STABLE |
         DEFRAG_VOLUME_FILES_AUTHORITATIVE |
         DEFRAG_VOLUME_EXTENTS_AUTHORITATIVE |
         DEFRAG_VOLUME_FREE_AUTHORITATIVE) ||
        volume->generation == 0)
        return DEFRAG_RESULT_NO_AUTHORITATIVE_SNAPSHOT;

    for (i = 0; i < volume->file_count; ++i) {
        defrag_result_t shape = defrag_validate_file_map_shape(&volume->files[i]);
        if (shape != DEFRAG_RESULT_OK)
            return shape;
        for (j = 0; j < volume->files[i].extent_count; ++j) {
            const defrag_extent_t *e = &volume->files[i].extents[j];
            uint64_t end;
            if (add_overflows_u64(e->physical_start, e->block_count))
                return DEFRAG_RESULT_CORRUPT_MAP;
            end = e->physical_start + e->block_count;
            if (end > volume->allocation_unit_count)
                return DEFRAG_RESULT_CORRUPT_MAP;
        }
    }
    for (i = 0; i < volume->free_range_count; ++i) {
        const defrag_free_range_t *r = &volume->free_ranges[i];
        uint64_t end;
        if (r->block_count == 0 || add_overflows_u64(r->physical_start, r->block_count))
            return DEFRAG_RESULT_CORRUPT_MAP;
        end = r->physical_start + r->block_count;
        if (end > volume->allocation_unit_count)
            return DEFRAG_RESULT_CORRUPT_MAP;
    }
    return DEFRAG_RESULT_OK;
}

/* Analyse a single immutable, authoritative snapshot. This routine only
 * allocates temporary memory; it never reads or writes a device itself. */
defrag_result_t defrag_analyze_volume(const defrag_volume_snapshot_t *volume, defrag_volume_analysis_t *out_analysis) {
    defrag_plan_t plan;
    defrag_result_t status;
    size_t i, total_extents = 0;
    defrag_volume_analysis_t result;

    if (!out_analysis)
        return DEFRAG_RESULT_INVALID_ARGUMENT;
    memset(out_analysis, 0, sizeof(*out_analysis));
    memset(&result, 0, sizeof(result));
    status = defrag_validate_volume_snapshot(volume);
    if (status != DEFRAG_RESULT_OK)
        return status;

    /* Reuse the planner's whole-snapshot overlap and free-space validation.
     * A plan is generated in memory and immediately discarded; no operation
     * is executed by this function. */
    memset(&plan, 0, sizeof(plan));
    status = defrag_plan_build(volume->files, volume->file_count,
                               volume->free_ranges, volume->free_range_count, &plan);
    if (status != DEFRAG_RESULT_OK)
        return status;

    for (i = 0; i < volume->file_count; ++i) {
        if (volume->files[i].extent_count > SIZE_MAX - total_extents) {
            defrag_plan_destroy(&plan);
            return DEFRAG_RESULT_OVERFLOW;
        }
        total_extents += volume->files[i].extent_count;
    }
    result.volume_id = volume->volume_id;
    result.generation = volume->generation;
    result.allocation_unit_count = volume->allocation_unit_count;
    result.allocation_unit_bytes = volume->allocation_unit_bytes;
    result.files_scanned = plan.stats.files_scanned;
    result.files_fragmented = plan.stats.files_fragmented;
    result.files_skipped = plan.stats.files_skipped;
    result.extents_scanned = total_extents;
    result.moves_plannable = plan.stats.moves_planned;
    result.files_without_contiguous_space = plan.stats.files_without_contiguous_space;
    defrag_plan_destroy(&plan);
    *out_analysis = result;
    return DEFRAG_RESULT_OK;
}

/* Produce a plan only when the provider explicitly says it is writable and
 * implements safe relocation. This still does not execute any move. */
defrag_result_t defrag_plan_build_for_volume(const defrag_volume_snapshot_t *volume,
                                            defrag_plan_t *out_plan) {
    defrag_result_t status;
    if (!out_plan)
        return DEFRAG_RESULT_INVALID_ARGUMENT;
    memset(out_plan, 0, sizeof(*out_plan));
    status = defrag_validate_volume_snapshot(volume);
    if (status != DEFRAG_RESULT_OK)
        return status;
    if (volume->flags & DEFRAG_VOLUME_READ_ONLY)
        return DEFRAG_RESULT_READ_ONLY;
    if (!(volume->flags & DEFRAG_VOLUME_RELOCATION_CAPABLE))
        return DEFRAG_RESULT_UNSAFE_VOLUME;
    return defrag_plan_build(volume->files, volume->file_count,
                             volume->free_ranges, volume->free_range_count, out_plan);
}

const char *defrag_result_string(defrag_result_t result) {
    switch (result) {
        case DEFRAG_RESULT_OK: return "success";
        case DEFRAG_RESULT_INVALID_ARGUMENT: return "invalid argument";
        case DEFRAG_RESULT_CORRUPT_MAP: return "corrupt or inconsistent allocation map";
        case DEFRAG_RESULT_OUT_OF_MEMORY: return "out of memory";
        case DEFRAG_RESULT_OVERFLOW: return "integer or allocation-size overflow";
        case DEFRAG_RESULT_UNSUPPORTED_FS: return "unsupported or non-disk filesystem";
        case DEFRAG_RESULT_UNSAFE_VOLUME: return "volume state or backend capabilities are unsafe";
        case DEFRAG_RESULT_NO_AUTHORITATIVE_SNAPSHOT: return "no stable authoritative filesystem snapshot";
        case DEFRAG_RESULT_READ_ONLY: return "volume is read-only";
        default: return "unknown defrag error";
    }
}

/* Serialize fields explicitly: CRC must not depend on structure padding/ABI. */
static size_t encode_record_without_crc(const defrag_journal_record_t *record,
                                        uint8_t out[56]) {
    size_t offset = 0;
    unsigned i;
#define PUT_FIELD(value, bytes) do { \
    uint64_t local_value = (uint64_t)(value); \
    for (i = 0; i < (bytes); ++i) { \
        out[offset++] = (uint8_t)(local_value & 0xffu); \
        local_value >>= 8; \
    } \
} while (0)
    PUT_FIELD(record->magic, 4);
    PUT_FIELD(record->version, 2);
    PUT_FIELD(record->state, 2);
    PUT_FIELD(record->sequence, 8);
    PUT_FIELD(record->file_id, 8);
    PUT_FIELD(record->expected_generation, 8);
    PUT_FIELD(record->destination_start, 8);
    PUT_FIELD(record->block_count, 8);
    PUT_FIELD(record->source_layout_hash, 8);
#undef PUT_FIELD
    return offset;
}

uint32_t defrag_journal_crc32(const defrag_journal_record_t *record) {
    uint8_t bytes[56];
    size_t length, i;
    uint32_t crc = UINT32_C(0xffffffff);
    if (!record)
        return 0;
    length = encode_record_without_crc(record, bytes);
    for (i = 0; i < length; ++i) {
        unsigned bit;
        crc ^= bytes[i];
        for (bit = 0; bit < 8; ++bit)
            crc = (crc >> 1) ^ (UINT32_C(0xedb88320) & (uint32_t)-(int32_t)(crc & 1u));
    }
    return ~crc;
}

int defrag_journal_seal(defrag_journal_record_t *record) {
    if (!record || record->magic != DEFRAG_JOURNAL_MAGIC ||
        record->version != DEFRAG_JOURNAL_VERSION ||
        record->state == DEFRAG_TXN_EMPTY || record->state > DEFRAG_TXN_ABORTED ||
        record->sequence == 0 || record->block_count == 0 ||
        record->block_count > UINT64_MAX - record->destination_start)
        return -1;
    record->crc32 = defrag_journal_crc32(record);
    return 0;
}

int defrag_journal_validate(const defrag_journal_record_t *record) {
    if (!record || record->magic != DEFRAG_JOURNAL_MAGIC ||
        record->version != DEFRAG_JOURNAL_VERSION ||
        record->state == DEFRAG_TXN_EMPTY || record->state > DEFRAG_TXN_ABORTED ||
        record->sequence == 0 || record->block_count == 0 ||
        record->block_count > UINT64_MAX - record->destination_start)
        return 0;
    return record->crc32 == defrag_journal_crc32(record);
}

void defrag_journal_init(defrag_journal_record_t *record,uint64_t file_id, uint64_t expected_generation, uint64_t destination_start, uint64_t block_count, uint64_t source_layout_hash) {
    if (!record)
        return;
    memset(record, 0, sizeof(*record));
    record->magic = DEFRAG_JOURNAL_MAGIC;
    record->version = DEFRAG_JOURNAL_VERSION;
    record->state = DEFRAG_TXN_PREPARED;
    record->sequence = 1;
    record->file_id = file_id;
    record->expected_generation = expected_generation;
    record->destination_start = destination_start;
    record->block_count = block_count;
    record->source_layout_hash = source_layout_hash;
    (void)defrag_journal_seal(record);
}

static int transition_allowed(defrag_txn_state_t current, defrag_txn_state_t next) {
    if (next == DEFRAG_TXN_ABORTED) {
        return current == DEFRAG_TXN_PREPARED ||
               current == DEFRAG_TXN_DEST_RESERVED ||
               current == DEFRAG_TXN_DATA_COPIED ||
               current == DEFRAG_TXN_DATA_VERIFIED;
    }
    switch (current) {
        case DEFRAG_TXN_PREPARED: return next == DEFRAG_TXN_DEST_RESERVED;
        case DEFRAG_TXN_DEST_RESERVED: return next == DEFRAG_TXN_DATA_COPIED;
        case DEFRAG_TXN_DATA_COPIED: return next == DEFRAG_TXN_DATA_VERIFIED;
        case DEFRAG_TXN_DATA_VERIFIED: return next == DEFRAG_TXN_METADATA_COMMITTED;
        case DEFRAG_TXN_METADATA_COMMITTED: return next == DEFRAG_TXN_SOURCE_RELEASED;
        case DEFRAG_TXN_SOURCE_RELEASED: return next == DEFRAG_TXN_COMPLETE;
        default: return 0;
    }
}

int defrag_journal_transition(defrag_journal_record_t *record, defrag_txn_state_t next_state) {
    defrag_journal_record_t updated;
    if (!defrag_journal_validate(record) || next_state <= DEFRAG_TXN_EMPTY ||
        next_state > DEFRAG_TXN_ABORTED || record->sequence == UINT64_MAX ||
        !transition_allowed((defrag_txn_state_t)record->state, next_state))
        return -1;
    updated = *record;
    updated.state = (uint16_t)next_state;
    ++updated.sequence;
    if (defrag_journal_seal(&updated) != 0)
        return -1;
    *record = updated;
    return 0;
}

defrag_recovery_action_t defrag_journal_recovery_action(const defrag_journal_record_t *record) {
    if (!defrag_journal_validate(record))
        return DEFRAG_RECOVERY_INVALID_RECORD;
    switch ((defrag_txn_state_t)record->state) {
        case DEFRAG_TXN_PREPARED: return DEFRAG_RECOVERY_CLEAR_KEEP_SOURCE;
        case DEFRAG_TXN_DEST_RESERVED: return DEFRAG_RECOVERY_DISCARD_DEST_KEEP_SOURCE;
        case DEFRAG_TXN_DATA_COPIED: return DEFRAG_RECOVERY_VERIFY_DESTINATION;
        case DEFRAG_TXN_DATA_VERIFIED: return DEFRAG_RECOVERY_COMMIT_NEW_MAPPING;
        case DEFRAG_TXN_METADATA_COMMITTED: return DEFRAG_RECOVERY_RELEASE_OLD_EXTENTS;
        case DEFRAG_TXN_SOURCE_RELEASED: return DEFRAG_RECOVERY_MARK_COMPLETE;
        case DEFRAG_TXN_COMPLETE: return DEFRAG_RECOVERY_CLEAR_COMPLETED_RECORD;
        case DEFRAG_TXN_ABORTED: return DEFRAG_RECOVERY_DISCARD_DEST_KEEP_SOURCE;
        default: return DEFRAG_RECOVERY_INVALID_RECORD;
    }
}

const char *defrag_txn_state_string(defrag_txn_state_t state) {
    switch (state) {
        case DEFRAG_TXN_EMPTY: return "empty";
        case DEFRAG_TXN_PREPARED: return "prepared";
        case DEFRAG_TXN_DEST_RESERVED: return "destination-reserved";
        case DEFRAG_TXN_DATA_COPIED: return "data-copied";
        case DEFRAG_TXN_DATA_VERIFIED: return "data-verified";
        case DEFRAG_TXN_METADATA_COMMITTED: return "metadata-committed";
        case DEFRAG_TXN_SOURCE_RELEASED: return "source-released";
        case DEFRAG_TXN_COMPLETE: return "complete";
        case DEFRAG_TXN_ABORTED: return "aborted";
        default: return "invalid";
    }
}

const char *defrag_recovery_action_string(defrag_recovery_action_t action) {
    switch (action) {
        case DEFRAG_RECOVERY_INVALID_RECORD: return "reject invalid/corrupt journal; do not guess";
        case DEFRAG_RECOVERY_CLEAR_KEEP_SOURCE: return "clear journal; source remains authoritative";
        case DEFRAG_RECOVERY_DISCARD_DEST_KEEP_SOURCE: return "discard unreferenced destination; keep source";
        case DEFRAG_RECOVERY_VERIFY_DESTINATION: return "verify copied destination before choosing next step";
        case DEFRAG_RECOVERY_COMMIT_NEW_MAPPING: return "commit new mapping using filesystem transaction";
        case DEFRAG_RECOVERY_RELEASE_OLD_EXTENTS: return "release old extents after committed mapping";
        case DEFRAG_RECOVERY_MARK_COMPLETE: return "mark transaction complete";
        case DEFRAG_RECOVERY_CLEAR_COMPLETED_RECORD: return "clear completed journal record";
        default: return "invalid action";
    }
}

/* Backend dispatch is selected by the mounted filesystem at runtime. The
 * table is capability metadata; the current kernel does not export the file
 * extent, free-space, transactional-move or recovery callbacks yet. */
#if DEFRAG_HAS_TERRARIUM_MOUNT_API && !defined(TERRARIUM_DEFRAG_NO_MAIN)
#define DEFRAG_QUERY_PAGE_CAPACITY 32u

typedef struct defrag_resolved_volume {
    char source[256];
    char target[256];
    char filesystem[32];
    uint32_t block_device_id;
    uint32_t mount_flags;
    uint64_t mount_generation;
} defrag_resolved_volume_t;

typedef enum defrag_vfs_resolve_result {
    DEFRAG_VFS_RESOLVE_OK = 0,
    DEFRAG_VFS_RESOLVE_API_UNAVAILABLE = -1,
    DEFRAG_VFS_RESOLVE_NOT_MOUNTED = -2,
    DEFRAG_VFS_RESOLVE_INVALID = -3,
    DEFRAG_VFS_RESOLVE_TOO_MANY_MOUNTS = -4,
    DEFRAG_VFS_RESOLVE_AMBIGUOUS = -5
} defrag_vfs_resolve_result_t;

typedef struct defrag_loaded_snapshot {
    defrag_volume_snapshot_t snapshot;
    defrag_file_map_t *files;
    defrag_extent_t *extents;
    defrag_free_range_t *free_ranges;
} defrag_loaded_snapshot_t;

static size_t defrag_string_length(const char *s) {
    size_t n = 0;
    if (s) while (s[n]) ++n;
    return n;
}

static void defrag_copy_string(char *out, size_t capacity, const char *in) {
    size_t i = 0;
    if (!out || capacity == 0) return;
    if (in) while (in[i] && i + 1 < capacity) { out[i] = in[i]; ++i; }
    out[i] = '\0';
}

static int defrag_string_is_terminated(const char *s, size_t cap) {
    return s && memchr(s, '\0', cap) != NULL;
}

static int is_drive_letter_target(const char *s) {
    size_t length;
    if (!s)
        return 0;
    length = strlen(s);
    return length == 2 &&
           ((s[0] >= 'A' && s[0] <= 'Z') || (s[0] >= 'a' && s[0] <= 'z')) &&
           s[1] == ':';
}

static int defrag_path_is_canonical_absolute(const char *path) {
    size_t i, component_start, length;
    if (!path || path[0] != '/') return 0;
    length = defrag_string_length(path);
    if (length == 0) return 0;
    if (length == 1) return 1;
    if (path[length - 1] == '/') --length;
    if (length == 1) return 1;
    if (path[1] == '/') return 0;
    component_start = 1;
    for (i = 1; i <= length; ++i) {
        if (i == length || path[i] == '/') {
            size_t n = i - component_start;
            if (n == 0 || (n == 1 && path[component_start] == '.') ||
                (n == 2 && path[component_start] == '.' && path[component_start + 1] == '.'))
                return 0;
            component_start = i + 1;
        }
    }
    return 1;
}

static int defrag_parse_volume_id(const char *target, uint64_t *out_id) {
    const char *p;
    uint64_t value = 0;
    if (!target || !out_id || strncmp(target, "volume:", 7) != 0)
        return 0;
    p = target + 7;
    if (!*p) return -1;
    while (*p) {
        unsigned digit;
        if (*p < '0' || *p > '9') return -1;
        digit = (unsigned)(*p - '0');
        if (value > (UINT64_MAX - digit) / 10u) return -1;
        value = value * 10u + digit;
        ++p;
    }
    if (value == 0) return -1;
    *out_id = value;
    return 1;
}

static void defrag_resolved_from_info(const terra_fs_mount_info_t *info,
                                      defrag_resolved_volume_t *out) {
    memset(out, 0, sizeof(*out));
    defrag_copy_string(out->source, sizeof(out->source), info->source);
    defrag_copy_string(out->target, sizeof(out->target), info->target);
    defrag_copy_string(out->filesystem, sizeof(out->filesystem), info->type);
    out->block_device_id = info->block_device_id;
    out->mount_flags = info->flags;
    out->mount_generation = info->mount_generation;
}

/* Query through the new stable VFS metadata ABI. Drive letters are accepted
 * only if explicitly published as source/target aliases; this OS does not
 * invent Windows-style C:-to-device mappings. volume:<decimal-id> is explicit. */
static defrag_vfs_resolve_result_t defrag_vfs_resolve_target(
    const char *target, defrag_resolved_volume_t *out_volume, int *out_api_status) {
    terra_fs_mount_info_t info;
    uint64_t volume_id = 0;
    int volume_target, status;
    if (out_api_status) *out_api_status = 0;
    if (!target || !target[0] || !out_volume)
        return DEFRAG_VFS_RESOLVE_INVALID;
    memset(out_volume, 0, sizeof(*out_volume));

    volume_target = defrag_parse_volume_id(target, &volume_id);
    if (volume_target < 0)
        return DEFRAG_VFS_RESOLVE_INVALID;
    if (volume_target > 0) {
        status = terra_fs_resolve_volume_mount_info(volume_id, &info);
        if (out_api_status) *out_api_status = status;
        if (status == TERRA_STATUS_OK) {
            if (!defrag_string_is_terminated(info.source, sizeof(info.source)) ||
                !defrag_string_is_terminated(info.target, sizeof(info.target)) ||
                !defrag_string_is_terminated(info.type, sizeof(info.type)))
                return DEFRAG_VFS_RESOLVE_INVALID;
            defrag_resolved_from_info(&info, out_volume);
            return DEFRAG_VFS_RESOLVE_OK;
        }
        return status == TERRA_STATUS_NOSYS ? DEFRAG_VFS_RESOLVE_API_UNAVAILABLE :
               status == TERRA_STATUS_NOENT ? DEFRAG_VFS_RESOLVE_NOT_MOUNTED :
               DEFRAG_VFS_RESOLVE_API_UNAVAILABLE;
    }

    if (is_drive_letter_target(target) ||
        (target[0] == '/' && strncmp(target, "/dev/", 5) == 0)) {
        terra_fs_mount_info_t mounts[TERRA_FS_QUERY_MAX_MOUNTS];
        uint32_t count = 0, i;
        int best = -1;
        status = terra_fs_mount_info_list(mounts, TERRA_FS_QUERY_MAX_MOUNTS, &count);
        if (out_api_status) *out_api_status = status;
        if (status != TERRA_STATUS_OK) {
            if (status == TERRA_STATUS_NOSPC)
                return DEFRAG_VFS_RESOLVE_TOO_MANY_MOUNTS;
            return DEFRAG_VFS_RESOLVE_API_UNAVAILABLE;
        }
        for (i = 0; i < count; ++i) {
            int match;
            if (!defrag_string_is_terminated(mounts[i].source, sizeof(mounts[i].source)) ||
                !defrag_string_is_terminated(mounts[i].target, sizeof(mounts[i].target)) ||
                !defrag_string_is_terminated(mounts[i].type, sizeof(mounts[i].type)))
                return DEFRAG_VFS_RESOLVE_INVALID;
            if (!mounts[i].target[0] || !mounts[i].type[0]) continue;
            match = is_drive_letter_target(target)
                ? (name_equal_ci(target, mounts[i].source) || name_equal_ci(target, mounts[i].target))
                : (mounts[i].source[0] && strcmp(target, mounts[i].source) == 0);
            if (!match) continue;
            if (best >= 0 && (mounts[best].mount_slot != mounts[i].mount_slot ||
                mounts[best].mount_generation != mounts[i].mount_generation))
                return DEFRAG_VFS_RESOLVE_AMBIGUOUS;
            best = (int)i;
        }
        if (best >= 0) {
            defrag_resolved_from_info(&mounts[best], out_volume);
            return DEFRAG_VFS_RESOLVE_OK;
        }
        if (is_drive_letter_target(target) ||
            (target[0] == '/' && strncmp(target, "/dev/", 5) == 0))
            return DEFRAG_VFS_RESOLVE_NOT_MOUNTED;
    }

    if (!defrag_path_is_canonical_absolute(target))
        return DEFRAG_VFS_RESOLVE_INVALID;
    status = terra_fs_resolve_mount_info(target, &info);
    if (out_api_status) *out_api_status = status;
    if (status != TERRA_STATUS_OK) {
        if (status == TERRA_STATUS_NOSYS)
            return DEFRAG_VFS_RESOLVE_API_UNAVAILABLE;
        if (status == TERRA_STATUS_NOENT)
            return DEFRAG_VFS_RESOLVE_NOT_MOUNTED;
        return DEFRAG_VFS_RESOLVE_API_UNAVAILABLE;
    }
    if (!defrag_string_is_terminated(info.source, sizeof(info.source)) ||
        !defrag_string_is_terminated(info.target, sizeof(info.target)) ||
        !defrag_string_is_terminated(info.type, sizeof(info.type)))
        return DEFRAG_VFS_RESOLVE_INVALID;
    defrag_resolved_from_info(&info, out_volume);
    return DEFRAG_VFS_RESOLVE_OK;
}

static void defrag_loaded_snapshot_destroy(defrag_loaded_snapshot_t *loaded) {
    if (!loaded) return;
    free(loaded->files);
    free(loaded->extents);
    free(loaded->free_ranges);
    memset(loaded, 0, sizeof(*loaded));
}

static defrag_result_t defrag_fetch_file_records(const char *path, uint64_t generation,
    uint64_t expected_count, terra_defrag_file_record_t **out_records,
    size_t *out_count, int *out_api_status) {
    terra_defrag_file_record_t *records = NULL;
    size_t expected, total = 0;
    uint64_t cursor = 0;
    if (!out_records || !out_count || expected_count > SIZE_MAX / sizeof(*records))
        return DEFRAG_RESULT_OVERFLOW;
    *out_records = NULL; *out_count = 0;
    expected = (size_t)expected_count;
    if (expected == 0) return DEFRAG_RESULT_OK;
    records = (terra_defrag_file_record_t *)malloc(expected * sizeof(*records));
    if (!records) return DEFRAG_RESULT_OUT_OF_MEMORY;
    while (total < expected) {
        terra_defrag_file_record_t page_records[DEFRAG_QUERY_PAGE_CAPACITY];
        terra_fs_query_page_t page;
        size_t capacity = expected - total;
        int status;
        if (capacity > DEFRAG_QUERY_PAGE_CAPACITY) capacity = DEFRAG_QUERY_PAGE_CAPACITY;
        memset(&page, 0, sizeof(page));
        status = terra_fs_defrag_file_page(path, generation, cursor, page_records, capacity, &page);
        if (out_api_status) *out_api_status = status;
        if (status != TERRA_STATUS_OK || page.count > capacity || page.has_more > 1 ||
            (page.has_more && (page.next_cursor == cursor || page.count == 0))) {
            free(records);
            return status == TERRA_STATUS_OK ? DEFRAG_RESULT_CORRUPT_MAP : DEFRAG_RESULT_NO_AUTHORITATIVE_SNAPSHOT;
        }
        if ((size_t)page.count > expected - total || (page.has_more && total + page.count >= expected)) {
            free(records); return DEFRAG_RESULT_CORRUPT_MAP;
        }
        memcpy(records + total, page_records, (size_t)page.count * sizeof(*records));
        total += page.count;
        if (!page.has_more) break;
        cursor = page.next_cursor;
    }
    if (total != expected) { free(records); return DEFRAG_RESULT_CORRUPT_MAP; }
    *out_records = records; *out_count = total;
    return DEFRAG_RESULT_OK;
}

static defrag_result_t defrag_fetch_extent_records(const char *path, uint64_t generation,
    uint64_t expected_count, terra_defrag_extent_record_t **out_records,
    size_t *out_count, int *out_api_status) {
    terra_defrag_extent_record_t *records = NULL;
    size_t expected, total = 0;
    uint64_t cursor = 0;
    if (!out_records || !out_count || expected_count > SIZE_MAX / sizeof(*records))
        return DEFRAG_RESULT_OVERFLOW;
    *out_records = NULL; *out_count = 0;
    expected = (size_t)expected_count;
    if (expected == 0) return DEFRAG_RESULT_OK;
    records = (terra_defrag_extent_record_t *)malloc(expected * sizeof(*records));
    if (!records) return DEFRAG_RESULT_OUT_OF_MEMORY;
    while (total < expected) {
        terra_defrag_extent_record_t page_records[DEFRAG_QUERY_PAGE_CAPACITY];
        terra_fs_query_page_t page;
        size_t capacity = expected - total;
        int status;
        if (capacity > DEFRAG_QUERY_PAGE_CAPACITY) capacity = DEFRAG_QUERY_PAGE_CAPACITY;
        memset(&page, 0, sizeof(page));
        status = terra_fs_defrag_extent_page(path, generation, cursor, page_records, capacity, &page);
        if (out_api_status) *out_api_status = status;
        if (status != TERRA_STATUS_OK || page.count > capacity || page.has_more > 1 ||
            (page.has_more && (page.next_cursor == cursor || page.count == 0))) {
            free(records);
            return status == TERRA_STATUS_OK ? DEFRAG_RESULT_CORRUPT_MAP : DEFRAG_RESULT_NO_AUTHORITATIVE_SNAPSHOT;
        }
        if ((size_t)page.count > expected - total || (page.has_more && total + page.count >= expected)) {
            free(records); return DEFRAG_RESULT_CORRUPT_MAP;
        }
        memcpy(records + total, page_records, (size_t)page.count * sizeof(*records));
        total += page.count;
        if (!page.has_more) break;
        cursor = page.next_cursor;
    }
    if (total != expected) { free(records); return DEFRAG_RESULT_CORRUPT_MAP; }
    *out_records = records; *out_count = total;
    return DEFRAG_RESULT_OK;
}

static defrag_result_t defrag_fetch_free_records(const char *path, uint64_t generation,
    uint64_t expected_count, terra_defrag_free_range_record_t **out_records,
    size_t *out_count, int *out_api_status) {
    terra_defrag_free_range_record_t *records = NULL;
    size_t expected, total = 0;
    uint64_t cursor = 0;
    if (!out_records || !out_count || expected_count > SIZE_MAX / sizeof(*records))
        return DEFRAG_RESULT_OVERFLOW;
    *out_records = NULL; *out_count = 0;
    expected = (size_t)expected_count;
    if (expected == 0) return DEFRAG_RESULT_OK;
    records = (terra_defrag_free_range_record_t *)malloc(expected * sizeof(*records));
    if (!records) return DEFRAG_RESULT_OUT_OF_MEMORY;
    while (total < expected) {
        terra_defrag_free_range_record_t page_records[DEFRAG_QUERY_PAGE_CAPACITY];
        terra_fs_query_page_t page;
        size_t capacity = expected - total;
        int status;
        if (capacity > DEFRAG_QUERY_PAGE_CAPACITY) capacity = DEFRAG_QUERY_PAGE_CAPACITY;
        memset(&page, 0, sizeof(page));
        status = terra_fs_defrag_free_range_page(path, generation, cursor, page_records, capacity, &page);
        if (out_api_status) *out_api_status = status;
        if (status != TERRA_STATUS_OK || page.count > capacity || page.has_more > 1 ||
            (page.has_more && (page.next_cursor == cursor || page.count == 0))) {
            free(records);
            return status == TERRA_STATUS_OK ? DEFRAG_RESULT_CORRUPT_MAP : DEFRAG_RESULT_NO_AUTHORITATIVE_SNAPSHOT;
        }
        if ((size_t)page.count > expected - total || (page.has_more && total + page.count >= expected)) {
            free(records); return DEFRAG_RESULT_CORRUPT_MAP;
        }
        memcpy(records + total, page_records, (size_t)page.count * sizeof(*records));
        total += page.count;
        if (!page.has_more) break;
        cursor = page.next_cursor;
    }
    if (total != expected) { free(records); return DEFRAG_RESULT_CORRUPT_MAP; }
    *out_records = records; *out_count = total;
    return DEFRAG_RESULT_OK;
}

static defrag_result_t defrag_load_live_snapshot(const defrag_resolved_volume_t *resolved,
    defrag_loaded_snapshot_t *out_loaded, int *out_api_status) {
    terra_defrag_volume_info_t info;
    terra_defrag_file_record_t *file_records = NULL;
    terra_defrag_extent_record_t *extent_records = NULL;
    terra_defrag_free_range_record_t *free_records = NULL;
    size_t file_count = 0, extent_count = 0, free_count = 0;
    size_t i, offset = 0, sum_extents = 0;
    const uint32_t known_flags = DEFRAG_FILE_REGULAR | DEFRAG_FILE_SPARSE |
        DEFRAG_FILE_SHARED | DEFRAG_FILE_PINNED | DEFRAG_FILE_SYSTEM |
        DEFRAG_FILE_UNWRITABLE | DEFRAG_FILE_OPEN_FOR_WRITE;
    defrag_result_t result;
    int status;

    if (!resolved || !out_loaded) return DEFRAG_RESULT_INVALID_ARGUMENT;
    memset(out_loaded, 0, sizeof(*out_loaded));
    memset(&info, 0, sizeof(info));
    status = terra_fs_defrag_volume_info(resolved->target, &info);
    if (out_api_status) *out_api_status = status;
    if (status != TERRA_STATUS_OK) return DEFRAG_RESULT_NO_AUTHORITATIVE_SNAPSHOT;
    if (info.abi_version != TERRA_FS_QUERY_ABI_VERSION || info.volume_id == 0 ||
        info.generation == 0 || info.allocation_unit_bytes == 0 ||
        info.allocation_unit_count == 0 ||
        (info.flags & (TERRA_DEFRAG_SNAPSHOT_STABLE | TERRA_DEFRAG_SNAPSHOT_FILES_AUTHORITATIVE |
                       TERRA_DEFRAG_SNAPSHOT_EXTENTS_AUTHORITATIVE | TERRA_DEFRAG_SNAPSHOT_FREE_AUTHORITATIVE)) !=
        (TERRA_DEFRAG_SNAPSHOT_STABLE | TERRA_DEFRAG_SNAPSHOT_FILES_AUTHORITATIVE |
         TERRA_DEFRAG_SNAPSHOT_EXTENTS_AUTHORITATIVE | TERRA_DEFRAG_SNAPSHOT_FREE_AUTHORITATIVE))
        return DEFRAG_RESULT_NO_AUTHORITATIVE_SNAPSHOT;

    result = defrag_fetch_file_records(resolved->target, info.generation, info.file_count,
                                       &file_records, &file_count, out_api_status);
    if (result != DEFRAG_RESULT_OK) goto done;
    result = defrag_fetch_extent_records(resolved->target, info.generation, info.extent_count,
                                         &extent_records, &extent_count, out_api_status);
    if (result != DEFRAG_RESULT_OK) goto done;
    result = defrag_fetch_free_records(resolved->target, info.generation, info.free_range_count,
                                       &free_records, &free_count, out_api_status);
    if (result != DEFRAG_RESULT_OK) goto done;

    if (file_count > SIZE_MAX / sizeof(*out_loaded->files) ||
        extent_count > SIZE_MAX / sizeof(*out_loaded->extents) ||
        free_count > SIZE_MAX / sizeof(*out_loaded->free_ranges)) {
        result = DEFRAG_RESULT_OVERFLOW; goto done;
    }
    if (file_count) {
        out_loaded->files = (defrag_file_map_t *)calloc(file_count, sizeof(*out_loaded->files));
        if (!out_loaded->files) { result = DEFRAG_RESULT_OUT_OF_MEMORY; goto done; }
    }
    if (extent_count) {
        out_loaded->extents = (defrag_extent_t *)calloc(extent_count, sizeof(*out_loaded->extents));
        if (!out_loaded->extents) { result = DEFRAG_RESULT_OUT_OF_MEMORY; goto done; }
    }
    if (free_count) {
        out_loaded->free_ranges = (defrag_free_range_t *)calloc(free_count, sizeof(*out_loaded->free_ranges));
        if (!out_loaded->free_ranges) { result = DEFRAG_RESULT_OUT_OF_MEMORY; goto done; }
    }

    for (i = 0; i < file_count; ++i) {
        if (file_records[i].file_id == 0 || file_records[i].reserved != 0 ||
            (file_records[i].file_flags & ~known_flags) != 0 ||
            (i != 0 && file_records[i - 1].file_id >= file_records[i].file_id) ||
            file_records[i].extent_count > SIZE_MAX ||
            (file_records[i].logical_block_count == 0 && file_records[i].extent_count != 0) ||
            file_records[i].extent_count > SIZE_MAX - sum_extents) {
            result = DEFRAG_RESULT_CORRUPT_MAP; goto done;
        }
        sum_extents += (size_t)file_records[i].extent_count;
    }
    if (sum_extents != extent_count) { result = DEFRAG_RESULT_CORRUPT_MAP; goto done; }

    for (i = 0; i < file_count; ++i) {
        size_t n = (size_t)file_records[i].extent_count;
        size_t j;
        defrag_file_map_t *file = &out_loaded->files[i];
        file->file_id = file_records[i].file_id;
        file->generation = file_records[i].file_generation;
        file->block_count = file_records[i].logical_block_count;
        file->flags = file_records[i].file_flags;
        file->extent_count = n;
        file->extents = n ? &out_loaded->extents[offset] : NULL;
        for (j = 0; j < n; ++j) {
            const terra_defrag_extent_record_t *src = &extent_records[offset + j];
            defrag_extent_t *dst = &out_loaded->extents[offset + j];
            if (src->file_id != file->file_id || src->file_generation != file->generation ||
                src->file_block_count != file->block_count || src->file_flags != file->flags ||
                src->reserved != 0 || src->block_count == 0 ||
                (j != 0 && extent_records[offset + j - 1].logical_start >= src->logical_start)) {
                result = DEFRAG_RESULT_CORRUPT_MAP; goto done;
            }
            dst->logical_start = src->logical_start;
            dst->physical_start = src->physical_start;
            dst->block_count = src->block_count;
        }
        offset += n;
    }
    for (i = 0; i < free_count; ++i) {
        out_loaded->free_ranges[i].physical_start = free_records[i].physical_start;
        out_loaded->free_ranges[i].block_count = free_records[i].block_count;
    }

    out_loaded->snapshot.volume_id = info.volume_id;
    out_loaded->snapshot.generation = info.generation;
    {
        const defrag_fs_profile_t *profile = defrag_fs_profile_by_name(resolved->filesystem);
        out_loaded->snapshot.filesystem = profile ? profile->kind : DEFRAG_FS_UNKNOWN;
    }
    if (info.flags & TERRA_DEFRAG_SNAPSHOT_STABLE)
        out_loaded->snapshot.flags |= DEFRAG_VOLUME_SNAPSHOT_STABLE;
    if (info.flags & TERRA_DEFRAG_SNAPSHOT_FILES_AUTHORITATIVE)
        out_loaded->snapshot.flags |= DEFRAG_VOLUME_FILES_AUTHORITATIVE;
    if (info.flags & TERRA_DEFRAG_SNAPSHOT_EXTENTS_AUTHORITATIVE)
        out_loaded->snapshot.flags |= DEFRAG_VOLUME_EXTENTS_AUTHORITATIVE;
    if (info.flags & TERRA_DEFRAG_SNAPSHOT_FREE_AUTHORITATIVE)
        out_loaded->snapshot.flags |= DEFRAG_VOLUME_FREE_AUTHORITATIVE;
    if (resolved->mount_flags & (TERRA_FS_MOUNT_INFO_READONLY | TERRA_FS_MOUNT_INFO_BLOCK_READONLY))
        out_loaded->snapshot.flags |= DEFRAG_VOLUME_READ_ONLY;
    out_loaded->snapshot.allocation_unit_bytes = info.allocation_unit_bytes;
    out_loaded->snapshot.allocation_unit_count = info.allocation_unit_count;
    out_loaded->snapshot.files = out_loaded->files;
    out_loaded->snapshot.file_count = file_count;
    out_loaded->snapshot.free_ranges = out_loaded->free_ranges;
    out_loaded->snapshot.free_range_count = free_count;
    result = defrag_validate_volume_snapshot(&out_loaded->snapshot);
    if (result != DEFRAG_RESULT_OK) goto done;
    free(file_records); free(extent_records); free(free_records);
    return DEFRAG_RESULT_OK;

done:
    free(file_records); free(extent_records); free(free_records);
    defrag_loaded_snapshot_destroy(out_loaded);
    return result;
}
#endif /* DEFRAG_HAS_TERRARIUM_MOUNT_API */

#ifndef TERRARIUM_DEFRAG_NO_MAIN
static int defrag_test_check(int condition, const char *name) {
    printf("[%s] %s\n", condition ? "PASS" : "FAIL", name);
    return condition ? 0 : 1;
}

/* Built-in deterministic tests for the pure snapshot analyzer/planner. These
 * tests construct in-memory snapshots only and never access block devices. */
static int defrag_run_self_tests(void) {
    int failures = 0;
    const defrag_extent_t fragmented_extents[] = {
        { 0, 10, 2 }, { 2, 20, 2 }
    };
    const defrag_file_map_t fragmented_files[] = {
        { 77, 9, 4, DEFRAG_FILE_REGULAR, fragmented_extents, 2 }
    };
    const defrag_free_range_t free_ranges[] = { { 100, 12 }, { 150, 10 } };
    defrag_volume_snapshot_t snapshot;
    defrag_volume_analysis_t analysis;
    defrag_plan_t plan;
    defrag_file_analysis_t file_analysis;
    defrag_journal_record_t journal;

    memset(&snapshot, 0, sizeof(snapshot));
    snapshot.volume_id = 1;
    snapshot.generation = 42;
#if defined(TERRARIUM_FS_VFS) || defined(TERRARIUM_FS_GENERIC)
    snapshot.filesystem = DEFRAG_FS_EXFAT; /* self-test fixture, not runtime detection */
#else
    snapshot.filesystem = DEFRAG_BUILD_FS_KIND;
#endif
    snapshot.flags = DEFRAG_VOLUME_SNAPSHOT_STABLE |
                     DEFRAG_VOLUME_FILES_AUTHORITATIVE |
                     DEFRAG_VOLUME_EXTENTS_AUTHORITATIVE |
                     DEFRAG_VOLUME_FREE_AUTHORITATIVE;
    snapshot.allocation_unit_bytes = 4096;
    snapshot.allocation_unit_count = 256;
    snapshot.files = fragmented_files;
    snapshot.file_count = 1;
    snapshot.free_ranges = free_ranges;
    snapshot.free_range_count = sizeof(free_ranges) / sizeof(free_ranges[0]);

    failures += defrag_test_check(
        defrag_analyze_file(&fragmented_files[0], &file_analysis) == DEFRAG_RESULT_OK &&
        file_analysis.fragmented && file_analysis.physical_fragments == 2,
        "fragmented file extent analysis");
    failures += defrag_test_check(
        defrag_analyze_volume(&snapshot, &analysis) == DEFRAG_RESULT_OK &&
        analysis.files_scanned == 1 && analysis.files_fragmented == 1 &&
        analysis.extents_scanned == 2 && analysis.moves_plannable == 1,
        "stable volume snapshot analysis and planning statistics");
    failures += defrag_test_check(
        defrag_plan_build_for_volume(&snapshot, &plan) == DEFRAG_RESULT_UNSAFE_VOLUME,
        "relocation refused without explicit backend capability");

    snapshot.flags |= DEFRAG_VOLUME_RELOCATION_CAPABLE;
    memset(&plan, 0, sizeof(plan));
    if (defrag_plan_build_for_volume(&snapshot, &plan) == DEFRAG_RESULT_OK) {
        failures += defrag_test_check(
            plan.move_count == 1 && plan.moves[0].destination_start == 150,
            "deterministic best-fit destination selection");
        defrag_plan_destroy(&plan);
    } else {
        failures += defrag_test_check(0, "deterministic best-fit destination selection");
    }

    {
        const defrag_extent_t gap_extents[] = { { 0, 10, 1 }, { 2, 20, 1 } };
        const defrag_file_map_t gap_file = {
            2, 1, 3, DEFRAG_FILE_REGULAR, gap_extents, 2
        };
        failures += defrag_test_check(
            defrag_analyze_file(&gap_file, &file_analysis) == DEFRAG_RESULT_CORRUPT_MAP,
            "unmarked logical gap rejected");
    }
    {
        const defrag_extent_t sparse_extents[] = { { 0, 10, 1 }, { 3, 30, 2 } };
        const defrag_file_map_t sparse_file = {
            3, 1, 5, DEFRAG_FILE_REGULAR | DEFRAG_FILE_SPARSE,
            sparse_extents, 2
        };
        failures += defrag_test_check(
            defrag_analyze_file(&sparse_file, &file_analysis) == DEFRAG_RESULT_OK &&
            !file_analysis.eligible && file_analysis.skip_reason == DEFRAG_SKIP_SPARSE_OR_GAPPED,
            "sparse file skipped safely");
    }
    {
        const defrag_extent_t outside_extents[] = { { 0, 250, 10 } };
        const defrag_file_map_t outside_file = {
            4, 1, 10, DEFRAG_FILE_REGULAR, outside_extents, 1
        };
        defrag_volume_snapshot_t bad_snapshot = snapshot;
        bad_snapshot.flags &= ~DEFRAG_VOLUME_RELOCATION_CAPABLE;
        bad_snapshot.files = &outside_file;
        bad_snapshot.filesystem = snapshot.filesystem;
        failures += defrag_test_check(
            defrag_analyze_volume(&bad_snapshot, &analysis) == DEFRAG_RESULT_CORRUPT_MAP,
            "extent outside volume bounds rejected");
    }
    {
        const defrag_free_range_t overlapping_free[] = { { 100, 10 }, { 105, 3 } };
        defrag_volume_snapshot_t bad_snapshot = snapshot;
        bad_snapshot.flags &= ~DEFRAG_VOLUME_RELOCATION_CAPABLE;
        bad_snapshot.files = NULL;
        bad_snapshot.file_count = 0;
        bad_snapshot.free_ranges = overlapping_free;
        bad_snapshot.free_range_count = 2;
        failures += defrag_test_check(
            defrag_analyze_volume(&bad_snapshot, &analysis) == DEFRAG_RESULT_CORRUPT_MAP,
            "overlapping free-space ranges rejected");
    }

    defrag_journal_init(&journal, 10, 1, 100, 4, 55);
    failures += defrag_test_check(
        defrag_journal_validate(&journal) &&
        defrag_journal_transition(&journal, DEFRAG_TXN_DEST_RESERVED) == 0 &&
        defrag_journal_transition(&journal, DEFRAG_TXN_DATA_COPIED) == 0 &&
        defrag_journal_recovery_action(&journal) == DEFRAG_RECOVERY_VERIFY_DESTINATION,
        "journal CRC, transitions and recovery action");
    journal.crc32 ^= 1u;
    failures += defrag_test_check(!defrag_journal_validate(&journal),
                                  "corrupt journal checksum rejected");

    printf("defrag self-test: %s (%d failure(s))\n",
           failures == 0 ? "PASS" : "FAIL", failures);
    return failures == 0 ? 0 : 1;
}

static void print_help(void) {
    puts("TerrariumOS defrag - filesystem defragmentation utility (safe foundation)");
    puts("Usage:");
    puts("  defrag <mountpoint|drive:|volume:ID> [--analyse|--analyze] [--verbose]");
    puts("  defrag --list-filesystems");
    puts("  defrag --status");
    puts("  defrag --self-test");
    puts("  defrag --help");
    puts("");
    puts("Options:");
    puts("  --analyse, --analyze, /analyse   Request read-only fragmentation analysis");
    puts("  volume:ID                        Resolve an explicit filesystem volume ID");
    puts("  --system,   [Info]");
    puts("  --offline,  [Info]");
    puts("  --verbose                        Explain the selected backend and safety checks");
    puts("  --list-filesystems, --list       Show filesystem integration status");
    puts("  --status                         Alias for --list-filesystems");
    puts("  --self-test                      Run in-memory analyzer/planner tests");
    puts("  --help, -h, /?                   Show this help");
    puts("");
    puts("No filesystem is modified by this build.");
    puts("The current Terrarium kernel does not expose file extents/free ranges or");
    puts("transactional relocation/recovery operations.");
}

static int arg_is(const char *arg, const char *a, const char *b, const char *c) {
    return arg && ((a && strcmp(arg, a) == 0) || (b && strcmp(arg, b) == 0) || (c && strcmp(arg, c) == 0));
}

static void print_profiles(void) {
    size_t i, count;
    const defrag_fs_profile_t *profiles = defrag_fs_profiles(&count);
    puts("Filesystem integration status (compile-time selection is not proof of backend support):");
    printf("Configured build: %s\n", defrag_build_fs_name());
    puts("Name    Selected  Probe  Mount desc.  Extent scan   Safe move");
    puts("------  --------  -----  -----------  ------------  ---------");
    for (i = 0; i < count; ++i) {
        const defrag_fs_profile_t *p = &profiles[i];
        printf("%-7s %-8s  %-5s  %-11s  %-12s  %s\n",
               p->name,
               p->selected_for_build ? "yes" : "no",
               p->probe_code_present ? "yes" : "no",
               p->mount_descriptor_present ? "yes" : (p->kind == DEFRAG_FS_VFS ? "n/a" : "no"),
               p->extent_enumeration_available ? "available" : "no",
               p->safe_relocation_available ? "available" : "no");
        printf("        %s\n", p->detail);
    }
    puts("");
    puts("Important: VFS is a routing layer, not a filesystem. It must dispatch to the");
    puts("mounted filesystem's extent and transactional relocation backend.");
}

int main(int argc, char **argv) {
    const char *target = NULL;
    int analyse_only = 0;
    int verbose = 0;
    int i;

    if (argc < 2) {
        fprintf(stderr, "Usage: defrag <mountpoint|drive:|volume:ID> [--analyse] [--verbose] | --list-filesystems | --self-test\n");
        return 1;
    }

    if (arg_is(argv[1], "--help", "-h", "/?")) {
        print_help();
        return 0;
    }
    if (strcmp(argv[1], "--self-test") == 0)
        return defrag_run_self_tests();
    if (arg_is(argv[1], "--list-filesystems", "--list", "/list") ||
        arg_is(argv[1], "--status", NULL, NULL)) {
        print_profiles();
        return 0;
    }

    for (i = 1; i < argc; ++i) {
        const char *arg = argv[i];
        if (arg_is(arg, "--help", "-h", "/?")) {
            print_help();
            return 0;
        } else if (arg_is(arg, "--analyse", "--analyze", "/analyse") ||
                   strcmp(arg, "/analyze") == 0) {
            analyse_only = 1;
        } else if (strcmp(arg, "--verbose") == 0 || strcmp(arg, "/verbose") == 0) {
            verbose = 1;
        } else if (arg[0] == '-') {
            fprintf(stderr, "defrag: unknown option: %s\n", arg);
            fprintf(stderr, "Usage: defrag <mountpoint|drive:|volume:ID> [--analyse] [--verbose]\n");
            return 1;
        } else if (!target) {
            target = arg;
        } else {
            fprintf(stderr, "defrag: unexpected argument: %s\n", arg);
            return 1;
        }
    }

    if (!target) {
        fprintf(stderr, "Usage: defrag <mountpoint|drive:|volume:ID> [--analyse] [--verbose]\n");
        return 1;
    }

    fprintf(stderr, "defrag: build configured for '%s'.\n", defrag_build_fs_name());
#if DEFRAG_HAS_TERRARIUM_MOUNT_API
    {
        defrag_resolved_volume_t volume;
        int api_status = 0;
        defrag_vfs_resolve_result_t resolve_status =
            defrag_vfs_resolve_target(target, &volume, &api_status);
        if (resolve_status != DEFRAG_VFS_RESOLVE_OK) {
            switch (resolve_status) {
                case DEFRAG_VFS_RESOLVE_API_UNAVAILABLE:
                    fprintf(stderr, "defrag: Terrarium VFS query API failed or is unavailable (status %d).\n", api_status);
                    break;
                case DEFRAG_VFS_RESOLVE_NOT_MOUNTED:
                    fprintf(stderr, "defrag: target '%s' is not present in the published mount table.\n", target);
                    if (is_drive_letter_target(target))
                        fprintf(stderr, "defrag: terra_mount_entry_t has no drive-letter field; no alias is guessed.\n");
                    break;
                case DEFRAG_VFS_RESOLVE_INVALID:
                    fprintf(stderr, "defrag: target must be a canonical absolute path, volume:ID, or explicitly published drive alias.\n");
                    break;
                case DEFRAG_VFS_RESOLVE_TOO_MANY_MOUNTS:
                    fprintf(stderr, "defrag: mount table filled the query buffer; refusing to resolve against a possibly truncated list.\n");
                    break;
                case DEFRAG_VFS_RESOLVE_AMBIGUOUS:
                    fprintf(stderr, "defrag: multiple mount records match equally; refusing to guess a volume.\n");
                    break;
                default:
                    fprintf(stderr, "defrag: mount resolution failed with internal status %d.\n", (int)resolve_status);
                    break;
            }
            fprintf(stderr, "defrag: target '%s' was not modified.\n", target);
            if (verbose) print_profiles();
            return 2;
        }

        {
            const defrag_fs_profile_t *profile = defrag_fs_profile_by_name(volume.filesystem);
            const uint32_t required_flags = TERRA_FS_MOUNT_INFO_VOLUME_QUERY |
                TERRA_FS_MOUNT_INFO_FILE_QUERY | TERRA_FS_MOUNT_INFO_EXTENT_QUERY |
                TERRA_FS_MOUNT_INFO_FREE_RANGE_QUERY;
            defrag_loaded_snapshot_t loaded;
            defrag_volume_analysis_t analysis;
            defrag_result_t result;
            int query_status = 0;
            memset(&loaded, 0, sizeof(loaded));
            fprintf(stderr, "defrag: resolved filesystem '%s' at '%s' (source '%s', block device %u, mount generation %llu, flags 0x%x).\n",
                    volume.filesystem, volume.target, volume.source, volume.block_device_id,
                    (unsigned long long)volume.mount_generation, (unsigned)volume.mount_flags);
            if (!profile || profile->kind == DEFRAG_FS_UNKNOWN || profile->kind == DEFRAG_FS_VFS) {
                fprintf(stderr, "defrag: resolved mount does not identify a supported on-disk filesystem.\n");
                if (verbose) print_profiles();
                return 2;
            }
            if ((volume.mount_flags & required_flags) != required_flags) {
                fprintf(stderr, "defrag: this mounted filesystem does not advertise the complete read-only snapshot API (volume/files/extents/free ranges).\n");
                fprintf(stderr, "defrag: no real analysis was performed; the target was not modified.\n");
                if (verbose) print_profiles();
                return 2;
            }
            result = defrag_load_live_snapshot(&volume, &loaded, &query_status);
            if (result != DEFRAG_RESULT_OK) {
                fprintf(stderr, "defrag: could not obtain a complete, stable filesystem snapshot: %s (API status %d).\n",
                        defrag_result_string(result), query_status);
                fprintf(stderr, "defrag: no analysis result is being reported and the target was not modified.\n");
                if (verbose) print_profiles();
                return 2;
            }
            result = defrag_analyze_volume(&loaded.snapshot, &analysis);
            if (result != DEFRAG_RESULT_OK) {
                fprintf(stderr, "defrag: snapshot validation/analysis failed: %s.\n", defrag_result_string(result));
                defrag_loaded_snapshot_destroy(&loaded);
                return 2;
            }
            printf("Filesystem analysis: %s at %s\n", volume.filesystem, volume.target);
            printf("  Volume ID:             %llu\n", (unsigned long long)analysis.volume_id);
            printf("  Snapshot generation:   %llu\n", (unsigned long long)analysis.generation);
            printf("  Allocation unit:       %u bytes\n", analysis.allocation_unit_bytes);
            printf("  Addressable units:     %llu\n", (unsigned long long)analysis.allocation_unit_count);
            printf("  Files scanned:         %zu\n", analysis.files_scanned);
            printf("  Fragmented files:      %zu\n", analysis.files_fragmented);
            printf("  Files skipped:         %zu\n", analysis.files_skipped);
            printf("  Extents scanned:       %zu\n", analysis.extents_scanned);
            printf("  Contiguous placements: %zu\n", analysis.moves_plannable);
            printf("  No-space cases:        %zu\n", analysis.files_without_contiguous_space);
            defrag_loaded_snapshot_destroy(&loaded);
            if (!analyse_only) {
                fprintf(stderr, "defrag: analysis completed, but relocation execution and crash recovery are not implemented; no files were moved.\n");
                if (verbose) print_profiles();
                return 2;
            }
            return 0;
        }
        if (verbose) print_profiles();
        return 2;
    }
#else
    fprintf(stderr, "defrag: standalone build has no TerrariumOS mount API adapter enabled.\n");
    fprintf(stderr, "defrag: place this file at utilities/storage/defrag.c to auto-detect system/fs/mount.h.\n");
    fprintf(stderr, "defrag: target '%s' was not modified; live VFS discovery and snapshot enumeration are unavailable.\n", target);
    if (!analyse_only) { fprintf(stderr, "defrag: relocation is disabled because no filesystem backend can perform transactional moves and recovery.\n"); }
    if (verbose) print_profiles();
    return 2;
#endif
}
#endif /* TERRARIUM_DEFRAG_NO_MAIN */