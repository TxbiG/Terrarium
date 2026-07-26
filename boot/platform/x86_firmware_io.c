#include "x86_firmware_io.h"
#include "x86_bios_protocol.h"
#include "x86_uefi_protocol.h"

int terra_bios_int13_read(const terra_x86_disk_read_request_t *request) {
    if (!request || !request->buffer || request->sector_count == 0)
        return -1;
    return terra_bios_protocol_read_lba(request->lba, request->sector_count, request->buffer);
}

int terra_bios_e820_collect(terra_x86_platform_state_t *state) {
    return terra_bios_protocol_collect_memory(state);
}

int terra_bios_vbe_discover(terra_x86_platform_state_t *state) {
    return terra_bios_protocol_discover_framebuffer(state);
}

int terra_bios_ps2_read_key(terra_boot_key_t *out_key) {
    return terra_bios_protocol_read_key(out_key);
}

uint64_t terra_bios_find_rsdp(void) {
    return terra_bios_protocol_find_rsdp();
}

int terra_uefi_file_read(const char *path, uint64_t destination, uint64_t *length) {
    return terra_uefi_protocol_read_file(path, destination, length);
}

int terra_uefi_block_read(const terra_x86_disk_read_request_t *request) {
    if (!request || !request->buffer || request->sector_count == 0)
        return -1;
    return terra_uefi_protocol_read_blocks(request->lba, request->sector_count, request->buffer);
}

int terra_uefi_memory_map_collect(terra_x86_platform_state_t *state) {
    return terra_uefi_protocol_collect_memory(state);
}

int terra_uefi_gop_discover(terra_x86_platform_state_t *state) {
    return terra_uefi_protocol_discover_gop(state);
}

int terra_uefi_text_read_key(terra_boot_key_t *out_key) {
    return terra_uefi_protocol_read_key(out_key);
}

uint64_t terra_uefi_find_rsdp(void) {
    return terra_uefi_protocol_find_rsdp();
}

int terra_uefi_exit_boot_services(void) {
    return terra_uefi_protocol_exit_boot_services();
}
