#include "x86_bios_realmode.h"

#if defined(TERRA_BOOT_ENABLE_BIOS_REALMODE_BINDINGS)

extern int terra_bios_rm_int13_read(const terra_bios_int13_packet_t *packet);
extern int terra_bios_rm_e820_collect(terra_bios_e820_entry_t *entries, uint32_t capacity, uint32_t *out_count);
extern int terra_bios_rm_vbe_current_mode(terra_bios_vbe_mode_info_t *out_mode);
extern int terra_bios_rm_read_key(terra_boot_key_t *out_key);
extern uint64_t terra_bios_rm_find_rsdp(void);

static int bios_read_sectors(void *context, const terra_bios_int13_packet_t *packet) {
    (void)context;
    return terra_bios_rm_int13_read(packet);
}

static int bios_get_e820(void *context, terra_bios_e820_entry_t *entries, uint32_t capacity, uint32_t *out_count) {
    (void)context;
    return terra_bios_rm_e820_collect(entries, capacity, out_count);
}

static int bios_get_vbe(void *context, terra_bios_vbe_mode_info_t *out_mode) {
    (void)context;
    return terra_bios_rm_vbe_current_mode(out_mode);
}

static int bios_get_key(void *context, terra_boot_key_t *out_key) {
    (void)context;
    return terra_bios_rm_read_key(out_key);
}

static uint64_t bios_get_rsdp(void *context) {
    (void)context;
    return terra_bios_rm_find_rsdp();
}

void terra_bios_install_real_mode_bindings(void) {
    terra_bios_protocols_t protocols = {
        .read_sectors = bios_read_sectors,
        .get_e820 = bios_get_e820,
        .get_vbe = bios_get_vbe,
        .get_key = bios_get_key,
        .get_rsdp = bios_get_rsdp,
        .context = 0
    };
    terra_bios_install_protocols(&protocols);
}

#else

void terra_bios_install_real_mode_bindings(void) {
}

#endif
