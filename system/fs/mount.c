#include "mount.h"
#include "../status.h"

int terra_mount(const terra_mount_entry_t *entry) {
    (void)entry;
    return TERRA_STATUS_NOSYS;
}

int terra_unmount(const char *target) {
    (void)target;
    return TERRA_STATUS_NOSYS;
}

int terra_mount_list(terra_mount_entry_t *entries, unsigned long max_entries) {
    (void)entries;
    (void)max_entries;
    return TERRA_STATUS_NOSYS;
}
