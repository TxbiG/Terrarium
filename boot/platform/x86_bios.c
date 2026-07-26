#include "x86_platform.h"
#include "x86_firmware_io.h"
#include "x86_bios_realmode.h"

#include <stdint.h>

static int g_bios_initialized;

static void bios_init_emulator_fallback(void) {
    if (g_bios_initialized)
        return;
    g_bios_initialized = 1;

    terra_x86_platform_reset(TERRA_X86_FIRMWARE_BIOS);
    terra_bios_install_real_mode_bindings();
    if (terra_bios_e820_collect(terra_x86_platform_state()) == 0) {
        terra_bios_vbe_discover(terra_x86_platform_state());
        terra_x86_platform_set_rsdp(terra_bios_find_rsdp());
        return;
    }

    /* Conservative emulator map used until real INT 15h E820 collection is wired. */
    terra_x86_platform_add_memory_region(0x00000000ull, 0x0009FC00ull, TERRA_MEMORY_USABLE, 0);
    terra_x86_platform_add_memory_region(0x0009FC00ull, 0x00060400ull, TERRA_MEMORY_RESERVED, 0);
    terra_x86_platform_add_memory_region(0x00100000ull, 0x0FF00000ull, TERRA_MEMORY_USABLE, 0);
    terra_x86_platform_set_smp(1, 0);
}

int terra_platform_get_framebuffer(terra_boot_framebuffer_t *framebuffer) {
    bios_init_emulator_fallback();
    return terra_x86_platform_get_framebuffer(framebuffer);
}

terra_boot_key_t terra_platform_read_key(void *user) {
    bios_init_emulator_fallback();
    terra_boot_key_t key = TERRA_BOOT_KEY_NONE;
    (void)user;
    if (terra_bios_ps2_read_key(&key) == 0)
        return key;
    return terra_x86_platform_read_key(0);
}

int terra_platform_load_file(const char *path, uint64_t destination, uint64_t *length) {
    bios_init_emulator_fallback();
    return terra_x86_platform_load_file(path, destination, length);
}

int terra_platform_get_memory_map(terra_boot_info_t *handoff) {
    bios_init_emulator_fallback();
    return terra_x86_platform_get_memory_map(handoff);
}

uint64_t terra_platform_get_rsdp(void) {
    bios_init_emulator_fallback();
    return terra_x86_platform_get_rsdp();
}

uint64_t terra_platform_get_device_tree(void) {
    return 0;
}




