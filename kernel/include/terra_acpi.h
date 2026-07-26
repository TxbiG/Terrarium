#ifndef TERRARIUM_ACPI_H
#define TERRARIUM_ACPI_H

#include <bootloader.h>

#ifdef __cplusplus
extern "C" {
#endif

void terra_acpi_init(const terra_boot_info_t *boot_info);
int terra_acpi_is_ready(void);

#ifdef __cplusplus
}
#endif

#endif
