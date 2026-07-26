#include "../include/boot_platform.h"

int terra_platform_get_framebuffer(terra_boot_framebuffer_t *framebuffer) {
    if (framebuffer) {
        framebuffer->pixels = 0;
        framebuffer->width = 0;
        framebuffer->height = 0;
        framebuffer->pitch_pixels = 0;
    }
    return -1;
}

terra_boot_key_t terra_platform_read_key(void *user) {
    (void)user;
    return TERRA_BOOT_KEY_NONE;
}

int terra_platform_load_file(const char *path, uint64_t destination, uint64_t *length) {
    (void)path;
    (void)destination;
    if (length)
        *length = 0;
    return -1;
}

int terra_platform_get_memory_map(terra_boot_info_t *handoff) {
    if (!handoff)
        return -1;
    handoff->memory_map_count = 0;
    handoff->mmTotal = 0;
    return 0;
}

uint64_t terra_platform_get_rsdp(void) {
    return 0;
}

uint64_t terra_platform_get_device_tree(void) {
    return 0;
}

void terra_platform_enter_kernel(uint64_t entry, const terra_boot_info_t *handoff) {
    (void)entry;
    (void)handoff;
    for (;;)
        ;
}
