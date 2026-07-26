#ifndef TERRARIUM_SYSTEM_FS_MOUNT_H
#define TERRARIUM_SYSTEM_FS_MOUNT_H

typedef struct terra_mount_entry {
    char source[256];
    char target[256];
    char type[32];
    unsigned long flags;
} terra_mount_entry_t;

int terra_mount(const terra_mount_entry_t *entry);
int terra_unmount(const char *target);
int terra_mount_list(terra_mount_entry_t *entries, unsigned long max_entries);

#endif
