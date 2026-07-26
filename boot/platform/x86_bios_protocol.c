#include "x86_bios_protocol.h"

static terra_bios_protocols_t g_bios_protocols;

static uint32_t map_e820_type(uint32_t bios_type) {
    switch (bios_type) {
    case 1: return TERRA_MEMORY_USABLE;
    case 3: return TERRA_MEMORY_ACPI_RECLAIM;
    case 4: return TERRA_MEMORY_ACPI_NVS;
    case 5: return TERRA_MEMORY_BAD;
    case 2:
    default: return TERRA_MEMORY_RESERVED;
    }
}

void terra_bios_install_protocols(const terra_bios_protocols_t *protocols) {
    if (protocols)
        g_bios_protocols = *protocols;
}

const terra_bios_protocols_t *terra_bios_protocols(void) {
    return &g_bios_protocols;
}

int terra_bios_protocol_collect_memory(terra_x86_platform_state_t *state) {
    terra_bios_e820_entry_t entries[TERRA_BOOTLOADER_MAX_MEMMAP];
    uint32_t count = 0;
    if (!state || !g_bios_protocols.get_e820)
        return -1;
    if (g_bios_protocols.get_e820(g_bios_protocols.context, entries, TERRA_BOOTLOADER_MAX_MEMMAP, &count) != 0)
        return -2;
    for (uint32_t i = 0; i < count; ++i) {
        if (terra_x86_platform_add_memory_region(entries[i].base, entries[i].length, map_e820_type(entries[i].type), entries[i].attributes) != 0)
            return -3;
    }
    return count ? 0 : -4;
}

int terra_bios_protocol_discover_framebuffer(terra_x86_platform_state_t *state) {
    terra_bios_vbe_mode_info_t mode;
    terra_boot_framebuffer_t fb;
    if (!state || !g_bios_protocols.get_vbe)
        return -1;
    if (g_bios_protocols.get_vbe(g_bios_protocols.context, &mode) != 0)
        return -2;
    fb.pixels = (uint32_t *)(uintptr_t)mode.framebuffer;
    fb.width = mode.width;
    fb.height = mode.height;
    {
        uint32_t bytes_per_pixel = mode.bpp >= 8 ? (uint32_t)(mode.bpp / 8u) : 4u;
        if (bytes_per_pixel == 0)
            bytes_per_pixel = 4u;
        fb.pitch_pixels = mode.pitch / bytes_per_pixel;
    }
    return terra_x86_platform_set_framebuffer(&fb);
}

int terra_bios_protocol_read_key(terra_boot_key_t *out_key) {
    if (!out_key)
        return -1;
    *out_key = TERRA_BOOT_KEY_NONE;
    if (!g_bios_protocols.get_key)
        return -2;
    return g_bios_protocols.get_key(g_bios_protocols.context, out_key);
}

uint64_t terra_bios_protocol_find_rsdp(void) {
    if (!g_bios_protocols.get_rsdp)
        return 0;
    return g_bios_protocols.get_rsdp(g_bios_protocols.context);
}

int terra_bios_protocol_read_lba(uint64_t lba, uint32_t sector_count, void *buffer) {
    terra_bios_int13_packet_t packet;
    if (!buffer || sector_count == 0 || sector_count > 0xFFFFu || !g_bios_protocols.read_sectors)
        return -1;
    packet.size = sizeof(packet);
    packet.reserved = 0;
    packet.sector_count = (uint16_t)sector_count;
    packet.buffer_offset = (uint16_t)((uintptr_t)buffer & 0xFu);
    packet.buffer_segment = (uint16_t)(((uintptr_t)buffer >> 4) & 0xFFFFu);
    packet.lba = lba;
    return g_bios_protocols.read_sectors(g_bios_protocols.context, &packet);
}
