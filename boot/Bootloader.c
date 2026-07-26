#include "include/boot_config.h"
#include "include/boot_platform.h"
#include "core/gfx/GUI.h"

#include <stdint.h>

#define TERRA_BOOT_CONFIG_PATH "/boot/boot.cfg"
#define TERRA_BOOT_CONFIG_LOAD_ADDR 0x00080000ull
#define TERRA_KERNEL_LOAD_ADDR 0x00100000ull
#define TERRA_INITRD_LOAD_ADDR 0x04000000ull
#define TERRA_MAX_CONFIG_SIZE 65536ull

static void halt_forever(void) {
    for (;;)
        ;
}

static int load_config(terra_boot_config_t *config) {
    uint64_t config_len = 0;
    char *config_text = (char *)(uintptr_t)TERRA_BOOT_CONFIG_LOAD_ADDR;

    terra_boot_config_default(config);

    if (terra_platform_load_file(TERRA_BOOT_CONFIG_PATH, TERRA_BOOT_CONFIG_LOAD_ADDR, &config_len) != 0)
        return 0;

    if (config_len == 0 || config_len >= TERRA_MAX_CONFIG_SIZE)
        return -1;

    config_text[config_len] = '\0';
    return terra_boot_config_parse(config_text, config);
}

static const terra_boot_entry_t *select_entry(const terra_boot_config_t *config) {
    terra_boot_framebuffer_t framebuffer;
    terra_boot_menu_t menu;

    if (!config || config->entry_count == 0)
        return 0;

    if (config->mode == TERRA_BOOT_SINGLE)
        return terra_boot_select_entry(config);

    if (terra_platform_get_framebuffer(&framebuffer) != 0 || !framebuffer.pixels)
        return terra_boot_select_entry(config);

    terra_boot_menu_init(&menu, config, framebuffer, terra_platform_read_key, 0);
    return terra_boot_menu_run(&menu);
}

static int load_selected_images(
    const terra_boot_entry_t *entry,
    terra_boot_loaded_image_t *kernel,
    terra_boot_loaded_image_t *initrd) {
    uint64_t kernel_len = 0;
    uint64_t initrd_len = 0;

    if (!entry || !kernel || !initrd || entry->kernel[0] == '\0')
        return -1;

    if (terra_platform_load_file(entry->kernel, TERRA_KERNEL_LOAD_ADDR, &kernel_len) != 0)
        return -2;

    kernel->base = TERRA_KERNEL_LOAD_ADDR;
    kernel->length = kernel_len;
    kernel->entry = TERRA_KERNEL_LOAD_ADDR;
    kernel->name = "kernel";

    initrd->base = TERRA_INITRD_LOAD_ADDR;
    initrd->length = 0;
    initrd->entry = 0;
    initrd->name = "initrd";
    if (entry->initrd[0] != '\0' &&
        terra_platform_load_file(entry->initrd, TERRA_INITRD_LOAD_ADDR, &initrd_len) == 0)
        initrd->length = initrd_len;

    return 0;
}

static int boot_native_terrarium(
    const terra_boot_entry_t *entry,
    const terra_boot_loaded_image_t *kernel,
    const terra_boot_loaded_image_t *initrd) {
    terra_boot_info_t handoff;

    if (terra_boot_prepare_handoff(entry, kernel, initrd, &handoff) != 0)
        return -1;

    terra_boot_secure_finalize(&handoff);
    if (terra_boot_verify_finalized_handoff(&handoff) != 0)
        return -2;

    terra_platform_enter_kernel(kernel->entry, &handoff);
    return -3;
}

static int boot_entry(const terra_boot_entry_t *entry) {
    terra_boot_loaded_image_t kernel = {0};
    terra_boot_loaded_image_t initrd = {0};

    if (load_selected_images(entry, &kernel, &initrd) != 0)
        return -1;

    switch (entry->protocol) {
    case TERRA_BOOT_PROTOCOL_TERRARIUM:
    case TERRA_BOOT_PROTOCOL_EFI_STUB:
    case TERRA_BOOT_PROTOCOL_DEVICETREE:
        return boot_native_terrarium(entry, &kernel, &initrd);
    case TERRA_BOOT_PROTOCOL_MULTIBOOT1:
    case TERRA_BOOT_PROTOCOL_MULTIBOOT2:
        return boot_native_terrarium(entry, &kernel, &initrd);
    }

    return -1;
}

void loader_main(void) {
    terra_boot_config_t config;
    const terra_boot_entry_t *entry;

    if (load_config(&config) != 0)
        terra_boot_config_default(&config);

    entry = select_entry(&config);
    if (!entry)
        halt_forever();

    if (boot_entry(entry) != 0)
        halt_forever();

    halt_forever();
}