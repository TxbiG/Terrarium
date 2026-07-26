#include "x86_uefi_protocol.h"

static terra_uefi_protocols_t g_uefi_protocols;

static uint32_t map_uefi_memory_type(uint32_t type) {
    switch (type) {
    case TERRA_UEFI_CONVENTIONAL_MEMORY:
    case TERRA_UEFI_LOADER_CODE:
    case TERRA_UEFI_LOADER_DATA:
    case TERRA_UEFI_BOOT_SERVICES_CODE:
    case TERRA_UEFI_BOOT_SERVICES_DATA:
        return TERRA_MEMORY_USABLE;
    case TERRA_UEFI_ACPI_RECLAIM_MEMORY:
        return TERRA_MEMORY_ACPI_RECLAIM;
    case TERRA_UEFI_ACPI_MEMORY_NVS:
        return TERRA_MEMORY_ACPI_NVS;
    case TERRA_UEFI_UNUSABLE_MEMORY:
        return TERRA_MEMORY_BAD;
    case TERRA_UEFI_MEMORY_MAPPED_IO:
    case TERRA_UEFI_RESERVED_MEMORY_TYPE:
    default:
        return TERRA_MEMORY_RESERVED;
    }
}

void terra_uefi_install_protocols(const terra_uefi_protocols_t *protocols) {
    if (protocols)
        g_uefi_protocols = *protocols;
}

const terra_uefi_protocols_t *terra_uefi_protocols(void) {
    return &g_uefi_protocols;
}

int terra_uefi_protocol_read_file(const char *path, uint64_t destination, uint64_t *length) {
    if (!path || !path[0] || destination == 0 || !g_uefi_protocols.read_file)
        return -1;
    return g_uefi_protocols.read_file(g_uefi_protocols.context, path, destination, length);
}

int terra_uefi_protocol_read_blocks(uint64_t lba, uint32_t sector_count, void *buffer) {
    if (!buffer || sector_count == 0 || !g_uefi_protocols.read_blocks)
        return -1;
    return g_uefi_protocols.read_blocks(g_uefi_protocols.context, lba, sector_count, buffer);
}

int terra_uefi_protocol_collect_memory(terra_x86_platform_state_t *state) {
    terra_uefi_memory_descriptor_t descriptors[TERRA_BOOTLOADER_MAX_MEMMAP];
    uint32_t count = 0;
    uint64_t map_key = 0;
    if (!state || !g_uefi_protocols.get_memory_map)
        return -1;
    if (g_uefi_protocols.get_memory_map(g_uefi_protocols.context, descriptors, TERRA_BOOTLOADER_MAX_MEMMAP, &count, &map_key) != 0)
        return -2;
    g_uefi_protocols.last_map_key = map_key;
    for (uint32_t i = 0; i < count; ++i) {
        uint64_t bytes = descriptors[i].number_of_pages * 4096ull;
        if (terra_x86_platform_add_memory_region(descriptors[i].physical_start, bytes, map_uefi_memory_type(descriptors[i].type), (uint32_t)descriptors[i].attributes) != 0)
            return -3;
    }
    return count ? 0 : -4;
}

int terra_uefi_protocol_discover_gop(terra_x86_platform_state_t *state) {
    terra_uefi_gop_mode_info_t mode;
    terra_boot_framebuffer_t fb;
    if (!state || !g_uefi_protocols.get_gop)
        return -1;
    if (g_uefi_protocols.get_gop(g_uefi_protocols.context, &mode) != 0)
        return -2;
    fb.pixels = (uint32_t *)(uintptr_t)mode.framebuffer;
    fb.width = mode.width;
    fb.height = mode.height;
    fb.pitch_pixels = mode.pixels_per_scanline;
    return terra_x86_platform_set_framebuffer(&fb);
}

int terra_uefi_protocol_read_key(terra_boot_key_t *out_key) {
    if (!out_key)
        return -1;
    *out_key = TERRA_BOOT_KEY_NONE;
    if (!g_uefi_protocols.read_key)
        return -2;
    return g_uefi_protocols.read_key(g_uefi_protocols.context, out_key);
}

uint64_t terra_uefi_protocol_find_rsdp(void) {
    if (!g_uefi_protocols.get_acpi_rsdp)
        return 0;
    return g_uefi_protocols.get_acpi_rsdp(g_uefi_protocols.context);
}

int terra_uefi_protocol_exit_boot_services(void) {
    if (!g_uefi_protocols.exit_boot_services)
        return -1;
    return g_uefi_protocols.exit_boot_services(g_uefi_protocols.context, g_uefi_protocols.last_map_key);
}
