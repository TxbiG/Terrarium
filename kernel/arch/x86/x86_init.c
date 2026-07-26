#include "../arch.h"

/*
 * Keep this file as the small x86 boundary. The existing x86 implementation
 * can be connected here once its headers and dependencies are normalized.
 */

void x86_early_init(const terra_boot_info_t *boot_info) {
    (void)boot_info;
}

void x86_late_init(const terra_boot_info_t *boot_info) {
    (void)boot_info;
}
