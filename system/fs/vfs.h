#ifndef TERRARIUM_SYSTEM_FS_VFS_H
#define TERRARIUM_SYSTEM_FS_VFS_H

#include <stdint.h>

typedef struct terra_vfs_stat {
    uint64_t size;
    uint64_t blocks;
    uint32_t mode;
    uint32_t uid;
    uint32_t gid;
} terra_vfs_stat_t;

int terra_vfs_stat(const char *path, terra_vfs_stat_t *out_stat);
int terra_vfs_mkdir(const char *path, uint32_t mode);
int terra_vfs_unlink(const char *path);
int terra_vfs_rename(const char *old_path, const char *new_path);

#endif
