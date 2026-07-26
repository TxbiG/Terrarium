#include <bootloader.h>

terra_boot_info_t bootloader;

static uint32_t bootloader_checksum32(const void *data, uint32_t size) {
    const uint8_t *bytes = (const uint8_t *)data;
    uint32_t sum = 0;

    for (uint32_t i = 0; i < size; ++i)
        sum = (sum << 5) - sum + bytes[i];

    return sum;
}

static int bootloader_validate(const terra_boot_info_t *boot_info) {
    terra_boot_info_t copy;
    uint32_t expected;

    if (!boot_info)
        return -1;
    if (boot_info->magic != TERRA_BOOTLOADER_MAGIC)
        return -2;
    if (boot_info->abi_version != TERRA_BOOTLOADER_ABI_VERSION)
        return -3;
    if (boot_info->size < sizeof(terra_boot_info_t))
        return -4;
    if (boot_info->kernel_phys_base == 0 || boot_info->kernel_phys_end <= boot_info->kernel_phys_base)
        return -5;
    if (boot_info->memory_map_count == 0 || boot_info->memory_map_count > TERRA_BOOTLOADER_MAX_MEMMAP)
        return -6;
    if (boot_info->module_count > TERRA_BOOTLOADER_MAX_MODULES)
        return -7;

    for (uint32_t i = 0; i < boot_info->memory_map_count; ++i) {
        const terra_memory_map_entry_t *entry = &boot_info->memory_map[i];
        if (entry->length == 0 || entry->base + entry->length < entry->base)
            return -8;
    }

    for (uint32_t i = 0; i < boot_info->module_count; ++i) {
        const terra_module_t *module = &boot_info->modules[i];
        if (module->length == 0 || module->base + module->length < module->base)
            return -9;
        if (module->base < boot_info->kernel_phys_end &&
            boot_info->kernel_phys_base < module->base + module->length)
            return -10;
        for (uint32_t j = i + 1; j < boot_info->module_count; ++j) {
            const terra_module_t *other = &boot_info->modules[j];
            if (other->length == 0 || other->base + other->length < other->base)
                return -11;
            if (module->base < other->base + other->length &&
                other->base < module->base + module->length)
                return -12;
        }
    }

    copy = *boot_info;
    expected = copy.checksum;
    copy.checksum = 0;
    if (expected != bootloader_checksum32(&copy, sizeof(copy)))
        return -13;

    return 0;
}

void bootloader_import(const terra_boot_info_t *boot_info) {
    if (bootloader_validate(boot_info) != 0)
        return;

    bootloader = *boot_info;
    bootloader.smp = &bootloader.smp_storage;
}
