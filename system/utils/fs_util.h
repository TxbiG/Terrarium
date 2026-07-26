#ifndef TERRARIUM_SYSTEM_UTILS_FS_UTIL_H
#define TERRARIUM_SYSTEM_UTILS_FS_UTIL_H

int terra_fs_copy(const char *source, const char *target);
int terra_fs_remove_recursive(const char *path);
int terra_fs_ensure_dir(const char *path);

#endif
