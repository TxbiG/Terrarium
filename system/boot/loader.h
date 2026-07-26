#ifndef TERRARIUM_SYSTEM_BOOT_LOADER_H
#define TERRARIUM_SYSTEM_BOOT_LOADER_H

typedef struct terra_boot_entry {
    char name[64];
    char kernel[256];
    char initrd[256];
    char cmdline[256];
} terra_boot_entry_t;

int terra_loader_list(terra_boot_entry_t *entries, unsigned long max_entries);
int terra_loader_write_entry(const terra_boot_entry_t *entry);
int terra_loader_remove_entry(const char *name);

#endif
