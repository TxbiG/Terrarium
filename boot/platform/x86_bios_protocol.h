#ifndef TERRARIUM_BOOT_X86_BIOS_PROTOCOL_H
#define TERRARIUM_BOOT_X86_BIOS_PROTOCOL_H

#include <stdint.h>
#include "x86_platform.h"

typedef struct terra_bios_int13_packet {
    uint8_t size;
    uint8_t reserved;
    uint16_t sector_count;
    uint16_t buffer_offset;
    uint16_t buffer_segment;
    uint64_t lba;
} terra_bios_int13_packet_t;

typedef struct terra_bios_e820_entry {
    uint64_t base;
    uint64_t length;
    uint32_t type;
    uint32_t attributes;
} terra_bios_e820_entry_t;

typedef struct terra_bios_vbe_mode_info {
    uint64_t framebuffer;
    uint32_t width;
    uint32_t height;
    uint32_t pitch;
    uint16_t bpp;
    uint8_t red_shift;
    uint8_t red_size;
    uint8_t green_shift;
    uint8_t green_size;
    uint8_t blue_shift;
    uint8_t blue_size;
} terra_bios_vbe_mode_info_t;

typedef int (*terra_bios_read_sectors_fn)(void *context, const terra_bios_int13_packet_t *packet);
typedef int (*terra_bios_get_e820_fn)(void *context, terra_bios_e820_entry_t *entries, uint32_t capacity, uint32_t *out_count);
typedef int (*terra_bios_get_vbe_fn)(void *context, terra_bios_vbe_mode_info_t *out_mode);
typedef int (*terra_bios_get_key_fn)(void *context, terra_boot_key_t *out_key);
typedef uint64_t (*terra_bios_get_rsdp_fn)(void *context);

typedef struct terra_bios_protocols {
    terra_bios_read_sectors_fn read_sectors;
    terra_bios_get_e820_fn get_e820;
    terra_bios_get_vbe_fn get_vbe;
    terra_bios_get_key_fn get_key;
    terra_bios_get_rsdp_fn get_rsdp;
    void *context;
} terra_bios_protocols_t;

void terra_bios_install_protocols(const terra_bios_protocols_t *protocols);
const terra_bios_protocols_t *terra_bios_protocols(void);
int terra_bios_protocol_collect_memory(terra_x86_platform_state_t *state);
int terra_bios_protocol_discover_framebuffer(terra_x86_platform_state_t *state);
int terra_bios_protocol_read_key(terra_boot_key_t *out_key);
uint64_t terra_bios_protocol_find_rsdp(void);
int terra_bios_protocol_read_lba(uint64_t lba, uint32_t sector_count, void *buffer);

#endif
