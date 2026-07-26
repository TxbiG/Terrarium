#ifndef TERRARIUM_BOOTLOADER_H
#define TERRARIUM_BOOTLOADER_H

#include <stdint.h>
#include <stddef.h>

#define TERRA_BOOTLOADER_MAGIC 0x54424F4Fu /* TBOO */
#define TERRA_BOOTLOADER_ABI_VERSION 1u
#define TERRA_BOOTLOADER_MAX_MEMMAP 128u
#define TERRA_BOOTLOADER_MAX_MODULES 16u
#define TERRA_BOOTLOADER_MAX_CMDLINE 256u

typedef enum terra_handoff_arch {
    TERRA_HANDOFF_ARCH_X86 = 0,
    TERRA_HANDOFF_ARCH_X86_64,
    TERRA_HANDOFF_ARCH_ARM32,
    TERRA_HANDOFF_ARCH_AARCH64,
    TERRA_HANDOFF_ARCH_RISCV32,
    TERRA_HANDOFF_ARCH_RISCV64
} terra_handoff_arch_t;

typedef enum terra_handoff_protocol {
    TERRA_HANDOFF_TERRARIUM = 0,
    TERRA_HANDOFF_MULTIBOOT1,
    TERRA_HANDOFF_MULTIBOOT2,
    TERRA_HANDOFF_EFI_STUB,
    TERRA_HANDOFF_DEVICETREE
} terra_handoff_protocol_t;

typedef enum terra_memory_type {
    TERRA_MEMORY_USABLE = 1,
    TERRA_MEMORY_RESERVED = 2,
    TERRA_MEMORY_ACPI_RECLAIM = 3,
    TERRA_MEMORY_ACPI_NVS = 4,
    TERRA_MEMORY_BAD = 5,
    TERRA_MEMORY_BOOTLOADER_RECLAIM = 6,
    TERRA_MEMORY_KERNEL_AND_MODULES = 7,
    TERRA_MEMORY_FRAMEBUFFER = 8
} terra_memory_type_t;

typedef struct terra_memory_map_entry {
    uint64_t base;
    uint64_t length;
    uint32_t type;
    uint32_t flags;
} terra_memory_map_entry_t;

typedef struct terra_framebuffer {
    uint64_t address;
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
} terra_framebuffer_t;

typedef struct terra_module {
    uint64_t base;
    uint64_t length;
    char name[64];
} terra_module_t;

typedef struct terra_smp_info {
    uint32_t cpu_count;
    uint32_t bsp_lapic_id;
    void *cpus;
} terra_smp_info_t;

typedef struct terra_boot_info {
    uint32_t magic;
    uint32_t abi_version;
    uint32_t size;
    uint32_t checksum;

    uint32_t arch;
    uint32_t protocol;
    uint64_t flags;

    uint64_t kernel_phys_base;
    uint64_t kernel_phys_end;
    uint64_t kernelVirtBase;
    uint64_t hhdmOffset;

    uint64_t mmTotal;
    uint32_t memory_map_count;
    terra_memory_map_entry_t memory_map[TERRA_BOOTLOADER_MAX_MEMMAP];

    terra_framebuffer_t framebuffer;

    uint64_t rsdp;
    uint64_t device_tree;

    uint32_t module_count;
    terra_module_t modules[TERRA_BOOTLOADER_MAX_MODULES];

    char cmdline[TERRA_BOOTLOADER_MAX_CMDLINE];

    uint32_t smpBspIndex;
    terra_smp_info_t smp_storage;
    terra_smp_info_t *smp;
} terra_boot_info_t;

extern terra_boot_info_t bootloader;

void bootloader_import(const terra_boot_info_t *boot_info);
void kmain(const terra_boot_info_t *boot_info);

#endif
