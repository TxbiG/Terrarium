#ifndef TERRARIUM_ARCH_H
#define TERRARIUM_ARCH_H

#include <bootloader.h>

#ifdef __cplusplus
extern "C" {
#endif

void arch_early_init(const terra_boot_info_t *boot_info);
void arch_late_init(const terra_boot_info_t *boot_info);
void arch_halt(void);
void arch_poweroff(void);
void arch_reboot(void);

#ifdef __cplusplus
}
#endif

#endif
