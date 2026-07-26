#include "../include/boot_config.h"

#include <stddef.h>
#include <stdint.h>

static int terra_streq(const char *a, const char *b) {
    while (*a && *b && *a == *b) {
        ++a;
        ++b;
    }
    return *a == *b;
}

static uint32_t terra_parse_u32(const char *text) {
    uint32_t value = 0;
    while (*text >= '0' && *text <= '9') {
        value = value * 10u + (uint32_t)(*text - '0');
        ++text;
    }
    return value;
}

static void terra_copy(char *dst, const char *src, uint32_t cap) {
    uint32_t i = 0;
    if (cap == 0)
        return;
    while (src[i] && i + 1 < cap) {
        dst[i] = src[i];
        ++i;
    }
    dst[i] = '\0';
}

static const char *terra_trim_left(const char *text) {
    while (*text == ' ' || *text == '\t' || *text == '\r')
        ++text;
    return text;
}

static void terra_trim_right(char *text) {
    uint32_t len = 0;
    while (text[len])
        ++len;
    while (len > 0 &&
           (text[len - 1] == ' ' || text[len - 1] == '\t' || text[len - 1] == '\r')) {
        text[len - 1] = '\0';
        --len;
    }
}

static terra_boot_arch_t terra_parse_arch(const char *value) {
    if (terra_streq(value, "x86"))
        return TERRA_BOOT_ARCH_X86;
    if (terra_streq(value, "x86_64"))
        return TERRA_BOOT_ARCH_X86_64;
    if (terra_streq(value, "arm") || terra_streq(value, "arm32"))
        return TERRA_BOOT_ARCH_ARM32;
    if (terra_streq(value, "aarch64") || terra_streq(value, "arm64"))
        return TERRA_BOOT_ARCH_AARCH64;
    if (terra_streq(value, "riscv32"))
        return TERRA_BOOT_ARCH_RISCV32;
    return TERRA_BOOT_ARCH_RISCV64;
}

static terra_boot_protocol_t terra_parse_protocol(const char *value) {
    if (terra_streq(value, "multiboot1"))
        return TERRA_BOOT_PROTOCOL_MULTIBOOT1;
    if (terra_streq(value, "multiboot2"))
        return TERRA_BOOT_PROTOCOL_MULTIBOOT2;
    if (terra_streq(value, "efi-stub"))
        return TERRA_BOOT_PROTOCOL_EFI_STUB;
    if (terra_streq(value, "devicetree"))
        return TERRA_BOOT_PROTOCOL_DEVICETREE;
    return TERRA_BOOT_PROTOCOL_TERRARIUM;
}

void terra_boot_config_default(terra_boot_config_t *config) {
    if (!config)
        return;

    config->mode = TERRA_BOOT_SINGLE;
    config->timeout_ms = 0;
    config->default_entry = 0;
    config->entry_count = 1;

    terra_copy(config->entries[0].name, "TerrariumOS", TERRA_BOOT_MAX_STRING);
    terra_copy(config->entries[0].kernel, "/boot/terrarium-kernel.bin", TERRA_BOOT_MAX_STRING);
    config->entries[0].initrd[0] = '\0';
    terra_copy(config->entries[0].cmdline, "root=auto quiet", TERRA_BOOT_MAX_STRING);
    config->entries[0].arch = TERRA_BOOT_ARCH_X86_64;
    config->entries[0].protocol = TERRA_BOOT_PROTOCOL_TERRARIUM;

    for (uint32_t i = 1; i < TERRA_BOOT_MAX_ENTRIES; ++i) {
        config->entries[i].name[0] = '\0';
        config->entries[i].kernel[0] = '\0';
        config->entries[i].initrd[0] = '\0';
        config->entries[i].cmdline[0] = '\0';
        config->entries[i].arch = TERRA_BOOT_ARCH_X86_64;
        config->entries[i].protocol = TERRA_BOOT_PROTOCOL_TERRARIUM;
    }
}

int terra_boot_config_parse(const char *text, terra_boot_config_t *config) {
    char line[192];
    uint32_t entry_index = 0;

    if (!text || !config)
        return -1;

    terra_boot_config_default(config);
    config->entry_count = 0;

    while (*text) {
        uint32_t i = 0;
        while (*text && *text != '\n' && i + 1 < sizeof(line))
            line[i++] = *text++;
        while (*text && *text != '\n')
            ++text;
        if (*text == '\n')
            ++text;
        line[i] = '\0';

        char *cursor = (char *)terra_trim_left(line);
        terra_trim_right(cursor);

        if (cursor[0] == '\0' || cursor[0] == '#')
            continue;

        if (terra_streq(cursor, "[entry]")) {
            if (config->entry_count >= TERRA_BOOT_MAX_ENTRIES)
                return -2;
            entry_index = config->entry_count++;
            config->entries[entry_index].name[0] = '\0';
            config->entries[entry_index].kernel[0] = '\0';
            config->entries[entry_index].initrd[0] = '\0';
            config->entries[entry_index].cmdline[0] = '\0';
            config->entries[entry_index].arch = TERRA_BOOT_ARCH_X86_64;
            config->entries[entry_index].protocol = TERRA_BOOT_PROTOCOL_TERRARIUM;
            continue;
        }

        char *equals = cursor;
        while (*equals && *equals != '=')
            ++equals;
        if (*equals != '=')
            continue;
        *equals = '\0';

        char *key = cursor;
        char *value = (char *)terra_trim_left(equals + 1);
        terra_trim_right(key);
        terra_trim_right(value);

        if (terra_streq(key, "mode")) {
            config->mode = terra_streq(value, "multi") ? TERRA_BOOT_MULTI : TERRA_BOOT_SINGLE;
        } else if (terra_streq(key, "timeout_ms")) {
            config->timeout_ms = terra_parse_u32(value);
        } else if (terra_streq(key, "default")) {
            config->default_entry = terra_parse_u32(value);
        } else if (config->entry_count > 0) {
            terra_boot_entry_t *entry = &config->entries[entry_index];
            if (terra_streq(key, "name"))
                terra_copy(entry->name, value, TERRA_BOOT_MAX_STRING);
            else if (terra_streq(key, "kernel"))
                terra_copy(entry->kernel, value, TERRA_BOOT_MAX_STRING);
            else if (terra_streq(key, "initrd"))
                terra_copy(entry->initrd, value, TERRA_BOOT_MAX_STRING);
            else if (terra_streq(key, "cmdline"))
                terra_copy(entry->cmdline, value, TERRA_BOOT_MAX_STRING);
            else if (terra_streq(key, "arch"))
                entry->arch = terra_parse_arch(value);
            else if (terra_streq(key, "protocol"))
                entry->protocol = terra_parse_protocol(value);
        }
    }

    if (config->entry_count == 0)
        terra_boot_config_default(config);
    if (config->default_entry >= config->entry_count)
        config->default_entry = 0;

    return 0;
}

const terra_boot_entry_t *terra_boot_select_entry(const terra_boot_config_t *config) {
    if (!config || config->entry_count == 0)
        return NULL;
    return &config->entries[config->default_entry];
}

const char *terra_boot_arch_name(terra_boot_arch_t arch) {
    switch (arch) {
    case TERRA_BOOT_ARCH_X86: return "x86";
    case TERRA_BOOT_ARCH_X86_64: return "x86_64";
    case TERRA_BOOT_ARCH_ARM32: return "arm32";
    case TERRA_BOOT_ARCH_AARCH64: return "aarch64";
    case TERRA_BOOT_ARCH_RISCV32: return "riscv32";
    case TERRA_BOOT_ARCH_RISCV64: return "riscv64";
    }
    return "unknown";
}

const char *terra_boot_protocol_name(terra_boot_protocol_t protocol) {
    switch (protocol) {
    case TERRA_BOOT_PROTOCOL_TERRARIUM: return "terrarium";
    case TERRA_BOOT_PROTOCOL_MULTIBOOT1: return "multiboot1";
    case TERRA_BOOT_PROTOCOL_MULTIBOOT2: return "multiboot2";
    case TERRA_BOOT_PROTOCOL_EFI_STUB: return "efi-stub";
    case TERRA_BOOT_PROTOCOL_DEVICETREE: return "devicetree";
    }
    return "unknown";
}
