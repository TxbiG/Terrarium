#ifndef TERRARIUM_BOOT_X86_FIRMWARE_IO_H
#define TERRARIUM_BOOT_X86_FIRMWARE_IO_H

#include <stdint.h>
#include "x86_platform.h"

typedef struct terra_x86_disk_read_request {
    uint64_t lba;
    uint32_t sector_count;
    void *buffer;
} terra_x86_disk_read_request_t;

int terra_bios_int13_read(const terra_x86_disk_read_request_t *request);
int terra_bios_e820_collect(terra_x86_platform_state_t *state);
int terra_bios_vbe_discover(terra_x86_platform_state_t *state);
int terra_bios_ps2_read_key(terra_boot_key_t *out_key);
uint64_t terra_bios_find_rsdp(void);

int terra_uefi_file_read(const char *path, uint64_t destination, uint64_t *length);
int terra_uefi_block_read(const terra_x86_disk_read_request_t *request);
int terra_uefi_memory_map_collect(terra_x86_platform_state_t *state);
int terra_uefi_gop_discover(terra_x86_platform_state_t *state);
int terra_uefi_text_read_key(terra_boot_key_t *out_key);
uint64_t terra_uefi_find_rsdp(void);
int terra_uefi_exit_boot_services(void);

#endif
