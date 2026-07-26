#ifndef TERRARIUM_BOOT_CONFIG_H
#define TERRARIUM_BOOT_CONFIG_H

#include <stdint.h>

#define TERRA_BOOT_MAX_ENTRIES 8
#define TERRA_BOOT_MAX_STRING 96

typedef enum terra_boot_mode {
    TERRA_BOOT_SINGLE = 0,
    TERRA_BOOT_MULTI = 1
} terra_boot_mode_t;

typedef enum terra_boot_arch {
    TERRA_BOOT_ARCH_X86 = 0,
    TERRA_BOOT_ARCH_X86_64,
    TERRA_BOOT_ARCH_ARM32,
    TERRA_BOOT_ARCH_AARCH64,
    TERRA_BOOT_ARCH_RISCV32,
    TERRA_BOOT_ARCH_RISCV64
} terra_boot_arch_t;

typedef enum terra_boot_protocol {
    TERRA_BOOT_PROTOCOL_TERRARIUM = 0,
    TERRA_BOOT_PROTOCOL_MULTIBOOT1,
    TERRA_BOOT_PROTOCOL_MULTIBOOT2,
    TERRA_BOOT_PROTOCOL_EFI_STUB,
    TERRA_BOOT_PROTOCOL_DEVICETREE
} terra_boot_protocol_t;

typedef struct terra_boot_entry {
    char name[TERRA_BOOT_MAX_STRING];
    char kernel[TERRA_BOOT_MAX_STRING];
    char initrd[TERRA_BOOT_MAX_STRING];
    char cmdline[TERRA_BOOT_MAX_STRING];
    terra_boot_arch_t arch;
    terra_boot_protocol_t protocol;
} terra_boot_entry_t;

typedef struct terra_boot_config {
    terra_boot_mode_t mode;
    uint32_t timeout_ms;
    uint32_t default_entry;
    uint32_t entry_count;
    terra_boot_entry_t entries[TERRA_BOOT_MAX_ENTRIES];
} terra_boot_config_t;

void terra_boot_config_default(terra_boot_config_t *config);
int terra_boot_config_parse(const char *text, terra_boot_config_t *config);
const terra_boot_entry_t *terra_boot_select_entry(const terra_boot_config_t *config);
const char *terra_boot_arch_name(terra_boot_arch_t arch);
const char *terra_boot_protocol_name(terra_boot_protocol_t protocol);

#endif
