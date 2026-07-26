#ifndef TERRARIUM_SYSTEM_FS_MKFS_H
#define TERRARIUM_SYSTEM_FS_MKFS_H

typedef struct terra_mkfs_options {
    const char *device;
    const char *type;
    const char *label;
} terra_mkfs_options_t;

int terra_mkfs(const terra_mkfs_options_t *options);

#endif
