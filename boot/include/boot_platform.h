#ifndef TERRARIUM_BOOT_PLATFORM_H
#define TERRARIUM_BOOT_PLATFORM_H

#include <stdint.h>
#include "boot_config.h"
#include "../core/gfx/GUI.h"
#include "../../kernel/include/bootloader.h"

typedef struct terra_boot_loaded_image {
    uint64_t base;
    uint64_t length;
    uint64_t entry;
    const char *name;
} terra_boot_loaded_image_t;

int terra_platform_get_framebuffer(terra_boot_framebuffer_t *framebuffer);
terra_boot_key_t terra_platform_read_key(void *user);
int terra_platform_load_file(const char *path, uint64_t destination, uint64_t *length);
int terra_platform_get_memory_map(terra_boot_info_t *handoff);
uint64_t terra_platform_get_rsdp(void);
uint64_t terra_platform_get_device_tree(void);
void terra_platform_enter_kernel(uint64_t entry, const terra_boot_info_t *handoff);

int terra_boot_prepare_handoff(
    const terra_boot_entry_t *entry,
    const terra_boot_loaded_image_t *kernel,
    const terra_boot_loaded_image_t *initrd,
    terra_boot_info_t *handoff);

int terra_boot_validate_handoff(const terra_boot_info_t *handoff);
void terra_boot_secure_finalize(terra_boot_info_t *handoff);
int terra_boot_verify_finalized_handoff(const terra_boot_info_t *handoff);

#endif