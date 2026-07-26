#include "../include/boot_platform.h"

#include <stdint.h>

static uint32_t terra_checksum32(const void *data, uint32_t size) {
    const uint8_t *bytes = (const uint8_t *)data;
    uint32_t sum = 0;

    for (uint32_t i = 0; i < size; ++i)
        sum = (sum << 5) - sum + bytes[i];

    return sum;
}

static void terra_zero(void *ptr, uint32_t size) {
    volatile uint8_t *bytes = (volatile uint8_t *)ptr;
    while (size--)
        *bytes++ = 0;
}

static int range_overflows(uint64_t base, uint64_t length) {
    return length == 0 || base + length < base;
}

static uint64_t range_end(uint64_t base, uint64_t length) {
    return base + length;
}

static int ranges_overlap(uint64_t a_base, uint64_t a_length, uint64_t b_base, uint64_t b_length) {
    if (range_overflows(a_base, a_length) || range_overflows(b_base, b_length))
        return 0;
    return a_base < range_end(b_base, b_length) && b_base < range_end(a_base, a_length);
}

static uint32_t map_arch(terra_boot_arch_t arch) {
    switch (arch) {
    case TERRA_BOOT_ARCH_X86: return TERRA_HANDOFF_ARCH_X86;
    case TERRA_BOOT_ARCH_X86_64: return TERRA_HANDOFF_ARCH_X86_64;
    case TERRA_BOOT_ARCH_ARM32: return TERRA_HANDOFF_ARCH_ARM32;
    case TERRA_BOOT_ARCH_AARCH64: return TERRA_HANDOFF_ARCH_AARCH64;
    case TERRA_BOOT_ARCH_RISCV32: return TERRA_HANDOFF_ARCH_RISCV32;
    case TERRA_BOOT_ARCH_RISCV64: return TERRA_HANDOFF_ARCH_RISCV64;
    }
    return TERRA_HANDOFF_ARCH_X86_64;
}

static uint32_t map_protocol(terra_boot_protocol_t protocol) {
    switch (protocol) {
    case TERRA_BOOT_PROTOCOL_TERRARIUM: return TERRA_HANDOFF_TERRARIUM;
    case TERRA_BOOT_PROTOCOL_MULTIBOOT1: return TERRA_HANDOFF_MULTIBOOT1;
    case TERRA_BOOT_PROTOCOL_MULTIBOOT2: return TERRA_HANDOFF_MULTIBOOT2;
    case TERRA_BOOT_PROTOCOL_EFI_STUB: return TERRA_HANDOFF_EFI_STUB;
    case TERRA_BOOT_PROTOCOL_DEVICETREE: return TERRA_HANDOFF_DEVICETREE;
    }
    return TERRA_HANDOFF_TERRARIUM;
}

static void copy_string(char *dst, const char *src, uint32_t cap) {
    uint32_t i = 0;
    if (!dst || cap == 0)
        return;
    if (!src)
        src = "";
    while (src[i] && i + 1 < cap) {
        dst[i] = src[i];
        ++i;
    }
    dst[i] = '\0';
}

static void add_module(terra_boot_info_t *handoff, const terra_boot_loaded_image_t *image, const char *fallback_name) {
    terra_module_t *module;

    if (!handoff || !image || image->length == 0)
        return;
    if (handoff->module_count >= TERRA_BOOTLOADER_MAX_MODULES)
        return;

    module = &handoff->modules[handoff->module_count++];
    module->base = image->base;
    module->length = image->length;
    copy_string(module->name, image->name ? image->name : fallback_name, sizeof(module->name));
}

int terra_boot_prepare_handoff(
    const terra_boot_entry_t *entry,
    const terra_boot_loaded_image_t *kernel,
    const terra_boot_loaded_image_t *initrd,
    terra_boot_info_t *handoff) {
    terra_boot_framebuffer_t menu_fb;

    if (!entry || !kernel || !handoff || kernel->entry == 0 || kernel->length == 0)
        return -1;
    if (range_overflows(kernel->base, kernel->length))
        return -1;

    terra_zero(handoff, sizeof(*handoff));

    handoff->magic = TERRA_BOOTLOADER_MAGIC;
    handoff->abi_version = TERRA_BOOTLOADER_ABI_VERSION;
    handoff->size = sizeof(*handoff);
    handoff->arch = map_arch(entry->arch);
    handoff->protocol = map_protocol(entry->protocol);

    handoff->kernel_phys_base = kernel->base;
    handoff->kernel_phys_end = kernel->base + kernel->length;
    handoff->kernelVirtBase = 0xFFFFFFFF80000000ull;
    handoff->hhdmOffset = 0xFFFF800000000000ull;

    handoff->rsdp = terra_platform_get_rsdp();
    handoff->device_tree = terra_platform_get_device_tree();
    handoff->smp = &handoff->smp_storage;
    handoff->smp_storage.cpu_count = 1;
    handoff->smp_storage.bsp_lapic_id = 0;

    copy_string(handoff->cmdline, entry->cmdline, TERRA_BOOTLOADER_MAX_CMDLINE);

    if (terra_platform_get_memory_map(handoff) != 0)
        return -2;

    add_module(handoff, initrd, "initrd");

    if (terra_platform_get_framebuffer(&menu_fb) == 0 && menu_fb.pixels) {
        handoff->framebuffer.address = (uint64_t)(uintptr_t)menu_fb.pixels;
        handoff->framebuffer.width = menu_fb.width;
        handoff->framebuffer.height = menu_fb.height;
        handoff->framebuffer.pitch = menu_fb.pitch_pixels * 4u;
        handoff->framebuffer.bpp = 32;
        handoff->framebuffer.red_shift = 16;
        handoff->framebuffer.red_size = 8;
        handoff->framebuffer.green_shift = 8;
        handoff->framebuffer.green_size = 8;
        handoff->framebuffer.blue_shift = 0;
        handoff->framebuffer.blue_size = 8;
    }

    return terra_boot_validate_handoff(handoff);
}

int terra_boot_validate_handoff(const terra_boot_info_t *handoff) {
    if (!handoff)
        return -1;
    if (handoff->magic != TERRA_BOOTLOADER_MAGIC)
        return -2;
    if (handoff->abi_version != TERRA_BOOTLOADER_ABI_VERSION)
        return -3;
    if (handoff->size < sizeof(terra_boot_info_t))
        return -4;
    if (handoff->kernel_phys_base == 0 || handoff->kernel_phys_end <= handoff->kernel_phys_base)
        return -5;
    if (range_overflows(handoff->kernel_phys_base, handoff->kernel_phys_end - handoff->kernel_phys_base))
        return -5;
    if (handoff->memory_map_count == 0 || handoff->memory_map_count > TERRA_BOOTLOADER_MAX_MEMMAP)
        return -6;
    if (handoff->module_count > TERRA_BOOTLOADER_MAX_MODULES)
        return -7;
    if (handoff->framebuffer.address != 0) {
        if (handoff->framebuffer.width == 0 || handoff->framebuffer.height == 0 || handoff->framebuffer.pitch == 0)
            return -12;
        if (handoff->framebuffer.bpp != 24 && handoff->framebuffer.bpp != 32)
            return -13;
    }
    if (!handoff->smp || handoff->smp->cpu_count == 0)
        return -14;

    for (uint32_t i = 0; i < handoff->memory_map_count; ++i) {
        const terra_memory_map_entry_t *entry = &handoff->memory_map[i];
        if (range_overflows(entry->base, entry->length))
            return -8;
    }

    for (uint32_t i = 0; i < handoff->module_count; ++i) {
        const terra_module_t *module = &handoff->modules[i];
        if (range_overflows(module->base, module->length))
            return -9;
        if (ranges_overlap(
                handoff->kernel_phys_base,
                handoff->kernel_phys_end - handoff->kernel_phys_base,
                module->base,
                module->length))
            return -10;
        for (uint32_t j = i + 1; j < handoff->module_count; ++j) {
            const terra_module_t *other = &handoff->modules[j];
            if (ranges_overlap(module->base, module->length, other->base, other->length))
                return -11;
        }
    }

    return 0;
}

void terra_boot_secure_finalize(terra_boot_info_t *handoff) {
    if (!handoff)
        return;

    handoff->checksum = 0;
    handoff->checksum = terra_checksum32(handoff, sizeof(*handoff));
}

int terra_boot_verify_finalized_handoff(const terra_boot_info_t *handoff) {
    terra_boot_info_t copy;
    uint32_t expected;

    if (terra_boot_validate_handoff(handoff) != 0)
        return -1;

    copy = *handoff;
    expected = copy.checksum;
    copy.checksum = 0;
    return expected == terra_checksum32(&copy, sizeof(copy)) ? 0 : -2;
}

