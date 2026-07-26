#ifndef TERRARIUM_BOOT_X86_PLATFORM_H
#define TERRARIUM_BOOT_X86_PLATFORM_H

#define TERRA_X86_PRELOADED_FILE_MAX 8u

#include <stdint.h>
#include "../include/boot_platform.h"

typedef enum terra_x86_firmware_kind {
    TERRA_X86_FIRMWARE_UNKNOWN = 0,
    TERRA_X86_FIRMWARE_BIOS,
    TERRA_X86_FIRMWARE_UEFI,
    TERRA_X86_FIRMWARE_EMULATOR
} terra_x86_firmware_kind_t;

typedef int (*terra_x86_load_file_fn)(void *context, const char *path, uint64_t destination, uint64_t *length);
typedef terra_boot_key_t (*terra_x86_read_key_fn)(void *context);
typedef void (*terra_x86_enter_kernel_fn)(void *context, uint64_t entry, const terra_boot_info_t *handoff);

typedef struct terra_x86_preloaded_file {
    char path[TERRA_BOOT_MAX_STRING];
    uint64_t source;
    uint64_t length;
} terra_x86_preloaded_file_t;

typedef struct terra_x86_platform_state {
    terra_x86_firmware_kind_t firmware;
    terra_boot_framebuffer_t framebuffer;
    terra_memory_map_entry_t memory_map[TERRA_BOOTLOADER_MAX_MEMMAP];
    uint32_t memory_map_count;
    uint64_t total_usable_memory;
    uint64_t rsdp;
    uint32_t cpu_count;
    uint32_t bsp_lapic_id;
    terra_x86_load_file_fn load_file;
    terra_x86_read_key_fn read_key;
    terra_x86_enter_kernel_fn enter_kernel;
    void *context;
    terra_x86_preloaded_file_t preloaded_files[TERRA_X86_PRELOADED_FILE_MAX];
    uint32_t preloaded_file_count;
} terra_x86_platform_state_t;

void terra_x86_platform_reset(terra_x86_firmware_kind_t firmware);
void terra_x86_platform_set_state(const terra_x86_platform_state_t *state);
terra_x86_platform_state_t *terra_x86_platform_state(void);
int terra_x86_platform_set_framebuffer(const terra_boot_framebuffer_t *framebuffer);
int terra_x86_platform_add_memory_region(uint64_t base, uint64_t length, uint32_t type, uint32_t flags);
void terra_x86_platform_set_rsdp(uint64_t rsdp);
void terra_x86_platform_set_smp(uint32_t cpu_count, uint32_t bsp_lapic_id);
void terra_x86_platform_set_file_loader(terra_x86_load_file_fn load_file, void *context);
int terra_x86_platform_register_file(const char *path, uint64_t source, uint64_t length);
void terra_x86_platform_set_key_reader(terra_x86_read_key_fn read_key, void *context);
void terra_x86_platform_set_kernel_enter(terra_x86_enter_kernel_fn enter_kernel, void *context);

int terra_x86_platform_get_framebuffer(terra_boot_framebuffer_t *framebuffer);
terra_boot_key_t terra_x86_platform_read_key(void *user);
int terra_x86_platform_load_file(const char *path, uint64_t destination, uint64_t *length);
int terra_x86_platform_get_memory_map(terra_boot_info_t *handoff);
uint64_t terra_x86_platform_get_rsdp(void);
uint64_t terra_x86_platform_get_device_tree(void);
void terra_x86_platform_enter_kernel(uint64_t entry, const terra_boot_info_t *handoff);

#endif

