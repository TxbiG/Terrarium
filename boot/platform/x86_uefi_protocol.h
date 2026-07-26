#ifndef TERRARIUM_BOOT_X86_UEFI_PROTOCOL_H
#define TERRARIUM_BOOT_X86_UEFI_PROTOCOL_H

#include <stdint.h>
#include "x86_platform.h"

typedef enum terra_uefi_memory_type {
    TERRA_UEFI_RESERVED_MEMORY_TYPE = 0,
    TERRA_UEFI_LOADER_CODE = 1,
    TERRA_UEFI_LOADER_DATA = 2,
    TERRA_UEFI_BOOT_SERVICES_CODE = 3,
    TERRA_UEFI_BOOT_SERVICES_DATA = 4,
    TERRA_UEFI_CONVENTIONAL_MEMORY = 7,
    TERRA_UEFI_ACPI_RECLAIM_MEMORY = 9,
    TERRA_UEFI_ACPI_MEMORY_NVS = 10,
    TERRA_UEFI_UNUSABLE_MEMORY = 11,
    TERRA_UEFI_MEMORY_MAPPED_IO = 12
} terra_uefi_memory_type_t;

typedef struct terra_uefi_memory_descriptor {
    uint32_t type;
    uint32_t pad;
    uint64_t physical_start;
    uint64_t virtual_start;
    uint64_t number_of_pages;
    uint64_t attributes;
} terra_uefi_memory_descriptor_t;

typedef struct terra_uefi_gop_mode_info {
    uint64_t framebuffer;
    uint32_t width;
    uint32_t height;
    uint32_t pixels_per_scanline;
    uint32_t pixel_format;
} terra_uefi_gop_mode_info_t;

typedef int (*terra_uefi_read_file_fn)(void *context, const char *path, uint64_t destination, uint64_t *length);
typedef int (*terra_uefi_read_blocks_fn)(void *context, uint64_t lba, uint32_t sector_count, void *buffer);
typedef int (*terra_uefi_get_memory_map_fn)(void *context, terra_uefi_memory_descriptor_t *descriptors, uint32_t capacity, uint32_t *out_count, uint64_t *out_map_key);
typedef int (*terra_uefi_get_gop_fn)(void *context, terra_uefi_gop_mode_info_t *out_mode);
typedef int (*terra_uefi_read_key_fn)(void *context, terra_boot_key_t *out_key);
typedef uint64_t (*terra_uefi_get_acpi_rsdp_fn)(void *context);
typedef int (*terra_uefi_exit_boot_services_fn)(void *context, uint64_t map_key);

typedef struct terra_uefi_protocols {
    terra_uefi_read_file_fn read_file;
    terra_uefi_read_blocks_fn read_blocks;
    terra_uefi_get_memory_map_fn get_memory_map;
    terra_uefi_get_gop_fn get_gop;
    terra_uefi_read_key_fn read_key;
    terra_uefi_get_acpi_rsdp_fn get_acpi_rsdp;
    terra_uefi_exit_boot_services_fn exit_boot_services;
    void *context;
    uint64_t last_map_key;
} terra_uefi_protocols_t;

void terra_uefi_install_protocols(const terra_uefi_protocols_t *protocols);
const terra_uefi_protocols_t *terra_uefi_protocols(void);
int terra_uefi_protocol_read_file(const char *path, uint64_t destination, uint64_t *length);
int terra_uefi_protocol_read_blocks(uint64_t lba, uint32_t sector_count, void *buffer);
int terra_uefi_protocol_collect_memory(terra_x86_platform_state_t *state);
int terra_uefi_protocol_discover_gop(terra_x86_platform_state_t *state);
int terra_uefi_protocol_read_key(terra_boot_key_t *out_key);
uint64_t terra_uefi_protocol_find_rsdp(void);
int terra_uefi_protocol_exit_boot_services(void);

#endif
