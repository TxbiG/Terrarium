#ifndef TERRARIUM_SYSTEM_BOOT_INITRAMFS_H
#define TERRARIUM_SYSTEM_BOOT_INITRAMFS_H

typedef struct terra_initramfs_file {
    const char *path;
    const void *data;
    unsigned long size;
} terra_initramfs_file_t;

int terra_initramfs_find(const char *path, terra_initramfs_file_t *out_file);
int terra_initramfs_extract(const char *target_root);

#endif
