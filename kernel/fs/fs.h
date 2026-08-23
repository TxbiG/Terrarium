#ifndef TERRARIUM_FS_H
#define TERRARIUM_FS_H

#include <stdint.h>
#include <stddef.h>
#include <bootloader.h>
#include <status.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _SSIZE_T_DEFINED
#define _SSIZE_T_DEFINED
typedef long long ssize_t;
#endif

#define TERRA_FS_MAX_PATH 128
#define TERRA_FS_MAX_NAME 32
#define TERRA_FS_MAX_TYPES 16
#define TERRA_FS_MAX_MOUNTS 16
#define TERRA_FS_MAX_NODES 128
#define TERRA_FS_MAX_HANDLES 64
#define TERRA_FS_INLINE_DATA 512
#define TERRA_FS_DISK_CONTEXT_SIZE 256
#define TERRA_VFS_O_RDONLY 0x0000u
#define TERRA_VFS_O_WRONLY 0x0001u
#define TERRA_VFS_O_RDWR   0x0002u
#define TERRA_VFS_O_CREAT  0x0100u
#define TERRA_VFS_O_EXCL   0x0200u
#define TERRA_VFS_O_TRUNC  0x0400u
#define TERRA_VFS_O_APPEND 0x0800u
#define TERRA_VFS_O_DIRECTORY 0x1000u


#define TERRA_FS_TYPE_PSEUDO (1u << 0)
#define TERRA_FS_TYPE_DISK (1u << 1)
#define TERRA_FS_TYPE_IMPORTED (1u << 2)
#define TERRA_FS_TYPE_MOUNTABLE (1u << 3)

typedef enum terra_fs_status {
    TERRA_FS_STATUS_UNSUPPORTED = 0,
    TERRA_FS_STATUS_READONLY = 1,
    TERRA_FS_STATUS_READWRITE = 2,
} terra_fs_status_t;

typedef enum terra_fs_error {
    TERRA_FS_OK = TERRA_STATUS_OK,
    TERRA_FS_ERR_INVAL = TERRA_STATUS_INVAL,
    TERRA_FS_ERR_NOENT = TERRA_STATUS_NOENT,
    TERRA_FS_ERR_EXIST = TERRA_STATUS_EXIST,
    TERRA_FS_ERR_NOSPC = TERRA_STATUS_NOSPC,
    TERRA_FS_ERR_NOTDIR = TERRA_STATUS_NOTDIR,
    TERRA_FS_ERR_ISDIR = TERRA_STATUS_ISDIR,
    TERRA_FS_ERR_RDONLY = TERRA_STATUS_ROFS,
    TERRA_FS_ERR_UNSUPPORTED = TERRA_STATUS_NOTSUP,
    TERRA_FS_ERR_BUSY = TERRA_STATUS_BUSY,
    TERRA_FS_ERR_BADF = TERRA_STATUS_BADF,
} terra_fs_error_t;

typedef enum terra_vfs_node_kind {
    TERRA_VFS_NODE_FILE = 1,
    TERRA_VFS_NODE_DIR = 2,
    TERRA_VFS_NODE_DEVICE = 3,
    TERRA_VFS_NODE_SYMLINK = 4,
} terra_vfs_node_kind_t;

typedef struct terra_vfs_stat {
    terra_vfs_node_kind_t kind;
    uint32_t mode;
    uint64_t size;
    uint32_t fs_type;
} terra_vfs_stat_t;

typedef struct terra_fs_type {
    uint32_t id;
    const char *name;
    terra_fs_status_t status;
    uint32_t flags;
} terra_fs_type_t;

typedef struct terra_vfs_node {
    uint32_t id;
    uint32_t parent;
    uint32_t fs_type;
    terra_vfs_node_kind_t kind;
    char name[TERRA_FS_MAX_NAME];
    uint8_t data[TERRA_FS_INLINE_DATA];
    uint64_t size;
    uint32_t mode;
    int used;
} terra_vfs_node_t;

typedef struct terra_vfs_mount {
    char path[TERRA_FS_MAX_PATH];
    uint32_t fs_type;
    uint32_t root_node;
    uint32_t block_device;
    const void *disk_driver;
    uint8_t disk_context[TERRA_FS_DISK_CONTEXT_SIZE];
    int readonly;
    int used;
} terra_vfs_mount_t;

typedef struct terra_vfs_file {
    uint32_t node;
    uint64_t offset;
    uint32_t flags;
    int used;
} terra_vfs_file_t;

void fs_init(const terra_boot_info_t *boot_info);
int fs_is_ready(void);
void fs_shutdown(void);

int terra_fs_register_type(const char *name, terra_fs_status_t status, uint32_t flags);
const terra_fs_type_t *terra_fs_type_by_name(const char *name);
const terra_fs_type_t *terra_fs_type_by_id(uint32_t id);
uint32_t terra_fs_type_count(void);

int terra_vfs_mount(const char *path, const char *type, int readonly);
int terra_vfs_mount_block(const char *path, const char *type, uint32_t block_device, int readonly);
int terra_vfs_mkdir(const char *path, uint32_t mode);
int terra_vfs_create(const char *path, terra_vfs_node_kind_t kind, uint32_t mode);
int terra_vfs_unlink(const char *path);
int terra_vfs_rename(const char *old_path, const char *new_path);
int terra_vfs_open(const char *path, uint32_t flags);
int terra_vfs_close(int fd);
ssize_t terra_vfs_read(int fd, void *buffer, size_t size);
ssize_t terra_vfs_write(int fd, const void *buffer, size_t size);
int terra_vfs_seek(int fd, uint64_t offset);
int terra_vfs_truncate(const char *path, uint64_t size);
int terra_vfs_stat(const char *path, terra_vfs_stat_t *out_stat);
int terra_vfs_list(const char *path, char *buffer, size_t size);

#ifdef __cplusplus
}
#endif

#endif
