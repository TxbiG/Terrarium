#include "vfs.h"
#include "../status.h"

int terra_vfs_stat(const char *path, terra_vfs_stat_t *out_stat) {
    (void)path;
    (void)out_stat;
    return TERRA_STATUS_NOSYS;
}

int terra_vfs_mkdir(const char *path, uint32_t mode) {
    (void)path;
    (void)mode;
    return TERRA_STATUS_NOSYS;
}

int terra_vfs_unlink(const char *path) {
    (void)path;
    return TERRA_STATUS_NOSYS;
}

int terra_vfs_rename(const char *old_path, const char *new_path) {
    (void)old_path;
    (void)new_path;
    return TERRA_STATUS_NOSYS;
}
