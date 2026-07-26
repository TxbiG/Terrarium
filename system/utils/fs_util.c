#include "fs_util.h"
#include "../status.h"

int terra_fs_copy(const char *source, const char *target) {
    (void)source;
    (void)target;
    return TERRA_STATUS_NOSYS;
}

int terra_fs_remove_recursive(const char *path) {
    (void)path;
    return TERRA_STATUS_NOSYS;
}

int terra_fs_ensure_dir(const char *path) {
    (void)path;
    return TERRA_STATUS_NOSYS;
}
