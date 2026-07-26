#ifndef TERRARIUM_SYSTEM_BOOT_BOOT_H
#define TERRARIUM_SYSTEM_BOOT_BOOT_H

typedef enum terra_system_boot_mode {
    TERRA_SYSTEM_BOOT_SINGLE = 0,
    TERRA_SYSTEM_BOOT_MULTI,
    TERRA_SYSTEM_BOOT_RECOVERY
} terra_system_boot_mode_t;

terra_system_boot_mode_t terra_boot_mode(void);
int terra_boot_set_next(terra_system_boot_mode_t mode, const char *entry_name);

#endif
