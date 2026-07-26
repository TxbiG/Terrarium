#include "x86_uefi_bindings.h"
#include "x86_uefi_protocol.h"

#include <stddef.h>
#include <stdint.h>

#if defined(__GNUC__) || defined(__clang__)
#define TERRA_EFIAPI __attribute__((ms_abi))
#else
#define TERRA_EFIAPI
#endif

typedef uint16_t terra_efi_char16_t;
typedef uint64_t terra_efi_status_t;
typedef uint64_t terra_efi_uintn_t;
typedef void *terra_efi_handle_t;

typedef struct terra_efi_guid {
    uint32_t data1;
    uint16_t data2;
    uint16_t data3;
    uint8_t data4[8];
} terra_efi_guid_t;

typedef struct terra_efi_table_header {
    uint64_t signature;
    uint32_t revision;
    uint32_t header_size;
    uint32_t crc32;
    uint32_t reserved;
} terra_efi_table_header_t;

typedef struct terra_efi_memory_descriptor {
    uint32_t type;
    uint32_t pad;
    uint64_t physical_start;
    uint64_t virtual_start;
    uint64_t number_of_pages;
    uint64_t attributes;
} terra_efi_memory_descriptor_t;

typedef struct terra_efi_simple_text_input_protocol terra_efi_simple_text_input_protocol_t;
typedef struct terra_efi_graphics_output_protocol terra_efi_graphics_output_protocol_t;
typedef struct terra_efi_loaded_image_protocol terra_efi_loaded_image_protocol_t;
typedef struct terra_efi_simple_file_system_protocol terra_efi_simple_file_system_protocol_t;
typedef struct terra_efi_file_protocol terra_efi_file_protocol_t;
typedef struct terra_efi_block_io_protocol terra_efi_block_io_protocol_t;

typedef struct terra_efi_input_key {
    uint16_t scan_code;
    terra_efi_char16_t unicode_char;
} terra_efi_input_key_t;

typedef struct terra_efi_configuration_table {
    terra_efi_guid_t vendor_guid;
    void *vendor_table;
} terra_efi_configuration_table_t;

typedef terra_efi_status_t (TERRA_EFIAPI *terra_efi_get_memory_map_fn)(
    terra_efi_uintn_t *memory_map_size,
    terra_efi_memory_descriptor_t *memory_map,
    terra_efi_uintn_t *map_key,
    terra_efi_uintn_t *descriptor_size,
    uint32_t *descriptor_version);
typedef terra_efi_status_t (TERRA_EFIAPI *terra_efi_exit_boot_services_fn)(terra_efi_handle_t image_handle, terra_efi_uintn_t map_key);
typedef terra_efi_status_t (TERRA_EFIAPI *terra_efi_locate_protocol_fn)(terra_efi_guid_t *protocol, void *registration, void **interface_out);
typedef terra_efi_status_t (TERRA_EFIAPI *terra_efi_open_protocol_fn)(terra_efi_handle_t handle, terra_efi_guid_t *protocol, void **interface_out, terra_efi_handle_t agent_handle, terra_efi_handle_t controller_handle, uint32_t attributes);
typedef void (TERRA_EFIAPI *terra_efi_copy_mem_fn)(void *destination, const void *source, terra_efi_uintn_t length);
typedef void (TERRA_EFIAPI *terra_efi_set_mem_fn)(void *buffer, terra_efi_uintn_t size, uint8_t value);

typedef struct terra_efi_boot_services {
    terra_efi_table_header_t hdr;
    void *raise_tpl;
    void *restore_tpl;
    void *allocate_pages;
    void *free_pages;
    terra_efi_get_memory_map_fn get_memory_map;
    void *allocate_pool;
    void *free_pool;
    void *create_event;
    void *set_timer;
    void *wait_for_event;
    void *signal_event;
    void *close_event;
    void *check_event;
    void *install_protocol_interface;
    void *reinstall_protocol_interface;
    void *uninstall_protocol_interface;
    void *handle_protocol;
    void *reserved;
    void *register_protocol_notify;
    void *locate_handle;
    void *locate_device_path;
    void *install_configuration_table;
    void *load_image;
    void *start_image;
    void *exit;
    void *unload_image;
    terra_efi_exit_boot_services_fn exit_boot_services;
    void *get_next_monotonic_count;
    void *stall;
    void *set_watchdog_timer;
    void *connect_controller;
    void *disconnect_controller;
    terra_efi_open_protocol_fn open_protocol;
    void *close_protocol;
    void *open_protocol_information;
    void *protocols_per_handle;
    void *locate_handle_buffer;
    terra_efi_locate_protocol_fn locate_protocol;
    void *install_multiple_protocol_interfaces;
    void *uninstall_multiple_protocol_interfaces;
    void *calculate_crc32;
    terra_efi_copy_mem_fn copy_mem;
    terra_efi_set_mem_fn set_mem;
} terra_efi_boot_services_t;

typedef struct terra_efi_system_table {
    terra_efi_table_header_t hdr;
    terra_efi_char16_t *firmware_vendor;
    uint32_t firmware_revision;
    terra_efi_handle_t console_in_handle;
    terra_efi_simple_text_input_protocol_t *con_in;
    terra_efi_handle_t console_out_handle;
    void *con_out;
    terra_efi_handle_t standard_error_handle;
    void *std_err;
    void *runtime_services;
    terra_efi_boot_services_t *boot_services;
    terra_efi_uintn_t number_of_table_entries;
    terra_efi_configuration_table_t *configuration_table;
} terra_efi_system_table_t;

struct terra_efi_simple_text_input_protocol {
    void *reset;
    terra_efi_status_t (TERRA_EFIAPI *read_key_stroke)(terra_efi_simple_text_input_protocol_t *self, terra_efi_input_key_t *key);
    void *wait_for_key;
};

typedef struct terra_efi_pixel_bitmask {
    uint32_t red_mask;
    uint32_t green_mask;
    uint32_t blue_mask;
    uint32_t reserved_mask;
} terra_efi_pixel_bitmask_t;

typedef struct terra_efi_graphics_output_mode_information {
    uint32_t version;
    uint32_t horizontal_resolution;
    uint32_t vertical_resolution;
    uint32_t pixel_format;
    terra_efi_pixel_bitmask_t pixel_information;
    uint32_t pixels_per_scan_line;
} terra_efi_graphics_output_mode_information_t;

typedef struct terra_efi_graphics_output_protocol_mode {
    uint32_t max_mode;
    uint32_t mode;
    terra_efi_graphics_output_mode_information_t *info;
    terra_efi_uintn_t size_of_info;
    uint64_t frame_buffer_base;
    terra_efi_uintn_t frame_buffer_size;
} terra_efi_graphics_output_protocol_mode_t;

struct terra_efi_graphics_output_protocol {
    void *query_mode;
    void *set_mode;
    void *blt;
    terra_efi_graphics_output_protocol_mode_t *mode;
};

struct terra_efi_loaded_image_protocol {
    uint32_t revision;
    terra_efi_handle_t parent_handle;
    terra_efi_system_table_t *system_table;
    terra_efi_handle_t device_handle;
    void *file_path;
    void *reserved;
    uint32_t load_options_size;
    void *load_options;
    void *image_base;
    uint64_t image_size;
    uint32_t image_code_type;
    uint32_t image_data_type;
    void *unload;
};

struct terra_efi_simple_file_system_protocol {
    uint64_t revision;
    terra_efi_status_t (TERRA_EFIAPI *open_volume)(terra_efi_simple_file_system_protocol_t *self, terra_efi_file_protocol_t **root);
};

struct terra_efi_file_protocol {
    uint64_t revision;
    terra_efi_status_t (TERRA_EFIAPI *open)(terra_efi_file_protocol_t *self, terra_efi_file_protocol_t **new_handle, terra_efi_char16_t *file_name, uint64_t open_mode, uint64_t attributes);
    terra_efi_status_t (TERRA_EFIAPI *close)(terra_efi_file_protocol_t *self);
    void *delete_file;
    terra_efi_status_t (TERRA_EFIAPI *read)(terra_efi_file_protocol_t *self, terra_efi_uintn_t *buffer_size, void *buffer);
};

typedef struct terra_efi_block_io_media {
    uint32_t media_id;
    uint8_t removable_media;
    uint8_t media_present;
    uint8_t logical_partition;
    uint8_t read_only;
    uint8_t write_caching;
    uint32_t block_size;
    uint32_t io_align;
    uint64_t last_block;
} terra_efi_block_io_media_t;

struct terra_efi_block_io_protocol {
    uint64_t revision;
    terra_efi_block_io_media_t *media;
    void *reset;
    terra_efi_status_t (TERRA_EFIAPI *read_blocks)(terra_efi_block_io_protocol_t *self, uint32_t media_id, uint64_t lba, terra_efi_uintn_t buffer_size, void *buffer);
};

typedef struct terra_uefi_binding_context {
    terra_efi_handle_t image_handle;
    terra_efi_system_table_t *system_table;
    terra_efi_boot_services_t *boot_services;
    terra_efi_loaded_image_protocol_t *loaded_image;
} terra_uefi_binding_context_t;

static terra_uefi_binding_context_t g_uefi_context;

static const terra_efi_guid_t g_loaded_image_guid = {0x5B1B31A1u, 0x9562u, 0x11D2u, {0x8E, 0x3F, 0x00, 0xA0, 0xC9, 0x69, 0x72, 0x3B}};
static const terra_efi_guid_t g_simple_fs_guid = {0x964E5B22u, 0x6459u, 0x11D2u, {0x8E, 0x39, 0x00, 0xA0, 0xC9, 0x69, 0x72, 0x3B}};
static const terra_efi_guid_t g_gop_guid = {0x9042A9DEu, 0x23DCu, 0x4A38u, {0x96, 0xFB, 0x7A, 0xDE, 0xD0, 0x80, 0x51, 0x6A}};
static const terra_efi_guid_t g_block_io_guid = {0x964E5B21u, 0x6459u, 0x11D2u, {0x8E, 0x39, 0x00, 0xA0, 0xC9, 0x69, 0x72, 0x3B}};
static const terra_efi_guid_t g_acpi20_guid = {0x8868E871u, 0xE4F1u, 0x11D3u, {0xBC, 0x22, 0x00, 0x80, 0xC7, 0x3C, 0x88, 0x81}};
static const terra_efi_guid_t g_acpi10_guid = {0xEB9D2D30u, 0x2D88u, 0x11D3u, {0x9A, 0x16, 0x00, 0x90, 0x27, 0x3F, 0xC1, 0x4D}};

#define TERRA_EFI_SUCCESS 0
#define TERRA_EFI_OPEN_PROTOCOL_GET_PROTOCOL 0x00000002u
#define TERRA_EFI_FILE_MODE_READ 0x0000000000000001ull

static int guid_equal(const terra_efi_guid_t *a, const terra_efi_guid_t *b) {
    const uint8_t *aa = (const uint8_t *)a;
    const uint8_t *bb = (const uint8_t *)b;
    for (uint32_t i = 0; i < sizeof(*a); ++i) {
        if (aa[i] != bb[i])
            return 0;
    }
    return 1;
}

static void ascii_to_efi_path(const char *path, terra_efi_char16_t *out, uint32_t capacity) {
    uint32_t i = 0;
    if (!out || capacity == 0)
        return;
    if (!path)
        path = "";
    while (*path == '/' || *path == '\\')
        ++path;
    while (path[i] && i + 1 < capacity) {
        out[i] = (path[i] == '/') ? (terra_efi_char16_t)'\\' : (terra_efi_char16_t)path[i];
        ++i;
    }
    out[i] = 0;
}

static int uefi_read_file(void *context, const char *path, uint64_t destination, uint64_t *length) {
    terra_uefi_binding_context_t *ctx = (terra_uefi_binding_context_t *)context;
    terra_efi_simple_file_system_protocol_t *fs = 0;
    terra_efi_file_protocol_t *root = 0;
    terra_efi_file_protocol_t *file = 0;
    terra_efi_char16_t efi_path[260];
    terra_efi_uintn_t size;
    void *interface_out = 0;

    if (!ctx || !ctx->boot_services || !ctx->loaded_image || !destination)
        return -1;
    if (!ctx->boot_services->open_protocol)
        return -2;

    if (ctx->boot_services->open_protocol(ctx->loaded_image->device_handle, (terra_efi_guid_t *)&g_simple_fs_guid, &interface_out, ctx->image_handle, 0, TERRA_EFI_OPEN_PROTOCOL_GET_PROTOCOL) != TERRA_EFI_SUCCESS)
        return -3;
    fs = (terra_efi_simple_file_system_protocol_t *)interface_out;
    if (!fs || fs->open_volume(fs, &root) != TERRA_EFI_SUCCESS)
        return -4;

    ascii_to_efi_path(path, efi_path, 260);
    if (root->open(root, &file, efi_path, TERRA_EFI_FILE_MODE_READ, 0) != TERRA_EFI_SUCCESS) {
        root->close(root);
        return -5;
    }

    size = length && *length ? (terra_efi_uintn_t)*length : (64ull * 1024ull * 1024ull);
    if (file->read(file, &size, (void *)(uintptr_t)destination) != TERRA_EFI_SUCCESS) {
        file->close(file);
        root->close(root);
        return -6;
    }
    if (length)
        *length = size;
    file->close(file);
    root->close(root);
    return 0;
}

static int uefi_read_blocks(void *context, uint64_t lba, uint32_t sector_count, void *buffer) {
    terra_uefi_binding_context_t *ctx = (terra_uefi_binding_context_t *)context;
    terra_efi_block_io_protocol_t *block = 0;
    void *interface_out = 0;
    if (!ctx || !ctx->boot_services || !ctx->loaded_image || !buffer || sector_count == 0)
        return -1;
    if (!ctx->boot_services->open_protocol)
        return -2;
    if (ctx->boot_services->open_protocol(ctx->loaded_image->device_handle, (terra_efi_guid_t *)&g_block_io_guid, &interface_out, ctx->image_handle, 0, TERRA_EFI_OPEN_PROTOCOL_GET_PROTOCOL) != TERRA_EFI_SUCCESS)
        return -3;
    block = (terra_efi_block_io_protocol_t *)interface_out;
    if (!block || !block->read_blocks)
        return -4;
    if (!block->media)
        return -5;
    return block->read_blocks(block, block->media->media_id, lba, (terra_efi_uintn_t)sector_count * block->media->block_size, buffer) == TERRA_EFI_SUCCESS ? 0 : -6;
}

static int uefi_get_memory_map(void *context, terra_uefi_memory_descriptor_t *descriptors, uint32_t capacity, uint32_t *out_count, uint64_t *out_map_key) {
    terra_uefi_binding_context_t *ctx = (terra_uefi_binding_context_t *)context;
    terra_efi_uintn_t map_size;
    terra_efi_uintn_t map_key = 0;
    terra_efi_uintn_t descriptor_size = 0;
    uint32_t descriptor_version = 0;
    uint8_t raw_map[sizeof(terra_efi_memory_descriptor_t) * TERRA_BOOTLOADER_MAX_MEMMAP + 1024];

    if (!ctx || !ctx->boot_services || !ctx->boot_services->get_memory_map || !descriptors || !out_count || capacity == 0)
        return -1;

    map_size = sizeof(raw_map);
    if (ctx->boot_services->get_memory_map(&map_size, (terra_efi_memory_descriptor_t *)raw_map, &map_key, &descriptor_size, &descriptor_version) != TERRA_EFI_SUCCESS)
        return -2;
    if (descriptor_size < sizeof(terra_efi_memory_descriptor_t))
        return -3;

    *out_count = 0;
    for (terra_efi_uintn_t offset = 0; offset + descriptor_size <= map_size && *out_count < capacity; offset += descriptor_size) {
        terra_efi_memory_descriptor_t *src = (terra_efi_memory_descriptor_t *)(raw_map + offset);
        descriptors[*out_count].type = src->type;
        descriptors[*out_count].pad = src->pad;
        descriptors[*out_count].physical_start = src->physical_start;
        descriptors[*out_count].virtual_start = src->virtual_start;
        descriptors[*out_count].number_of_pages = src->number_of_pages;
        descriptors[*out_count].attributes = src->attributes;
        ++*out_count;
    }
    if (out_map_key)
        *out_map_key = map_key;
    return *out_count ? 0 : -4;
}

static int uefi_get_gop(void *context, terra_uefi_gop_mode_info_t *out_mode) {
    terra_uefi_binding_context_t *ctx = (terra_uefi_binding_context_t *)context;
    terra_efi_graphics_output_protocol_t *gop = 0;
    void *interface_out = 0;
    if (!ctx || !ctx->boot_services || !ctx->boot_services->locate_protocol || !out_mode)
        return -1;
    if (ctx->boot_services->locate_protocol((terra_efi_guid_t *)&g_gop_guid, 0, &interface_out) != TERRA_EFI_SUCCESS)
        return -2;
    gop = (terra_efi_graphics_output_protocol_t *)interface_out;
    if (!gop || !gop->mode || !gop->mode->info)
        return -3;
    out_mode->framebuffer = gop->mode->frame_buffer_base;
    out_mode->width = gop->mode->info->horizontal_resolution;
    out_mode->height = gop->mode->info->vertical_resolution;
    out_mode->pixels_per_scanline = gop->mode->info->pixels_per_scan_line;
    out_mode->pixel_format = gop->mode->info->pixel_format;
    return 0;
}

static int uefi_read_key(void *context, terra_boot_key_t *out_key) {
    terra_uefi_binding_context_t *ctx = (terra_uefi_binding_context_t *)context;
    terra_efi_input_key_t key;
    if (!ctx || !ctx->system_table || !ctx->system_table->con_in || !ctx->system_table->con_in->read_key_stroke || !out_key)
        return -1;
    if (ctx->system_table->con_in->read_key_stroke(ctx->system_table->con_in, &key) != TERRA_EFI_SUCCESS)
        return -2;
    switch (key.scan_code) {
    case 0x01: *out_key = TERRA_BOOT_KEY_UP; break;
    case 0x02: *out_key = TERRA_BOOT_KEY_DOWN; break;
    default:
        if (key.scan_code == 0x17)
            *out_key = TERRA_BOOT_KEY_ESCAPE;
        else
            *out_key = (key.unicode_char == '\r') ? TERRA_BOOT_KEY_ENTER : TERRA_BOOT_KEY_NONE;
        break;
    }
    return 0;
}

static uint64_t uefi_get_acpi_rsdp(void *context) {
    terra_uefi_binding_context_t *ctx = (terra_uefi_binding_context_t *)context;
    if (!ctx || !ctx->system_table || !ctx->system_table->configuration_table)
        return 0;
    for (terra_efi_uintn_t i = 0; i < ctx->system_table->number_of_table_entries; ++i) {
        terra_efi_configuration_table_t *entry = &ctx->system_table->configuration_table[i];
        if (guid_equal(&entry->vendor_guid, &g_acpi20_guid) || guid_equal(&entry->vendor_guid, &g_acpi10_guid))
            return (uint64_t)(uintptr_t)entry->vendor_table;
    }
    return 0;
}

static int uefi_exit_boot_services(void *context, uint64_t map_key) {
    terra_uefi_binding_context_t *ctx = (terra_uefi_binding_context_t *)context;
    terra_efi_uintn_t map_size;
    terra_efi_uintn_t fresh_map_key = (terra_efi_uintn_t)map_key;
    terra_efi_uintn_t descriptor_size = 0;
    uint32_t descriptor_version = 0;
    uint8_t raw_map[sizeof(terra_efi_memory_descriptor_t) * TERRA_BOOTLOADER_MAX_MEMMAP + 1024];
    if (!ctx || !ctx->boot_services || !ctx->boot_services->exit_boot_services || !ctx->image_handle)
        return -1;
    if (ctx->boot_services->get_memory_map) {
        map_size = sizeof(raw_map);
        if (ctx->boot_services->get_memory_map(&map_size, (terra_efi_memory_descriptor_t *)raw_map, &fresh_map_key, &descriptor_size, &descriptor_version) != TERRA_EFI_SUCCESS)
            return -2;
    }
    return ctx->boot_services->exit_boot_services(ctx->image_handle, fresh_map_key) == TERRA_EFI_SUCCESS ? 0 : -3;
}

int terra_uefi_bind_firmware_services(void *image_handle, void *system_table) {
    terra_uefi_protocols_t protocols;
    void *loaded_image = 0;

    if (!image_handle || !system_table)
        return -1;
    g_uefi_context.image_handle = image_handle;
    g_uefi_context.system_table = (terra_efi_system_table_t *)system_table;
    g_uefi_context.boot_services = g_uefi_context.system_table->boot_services;
    if (!g_uefi_context.boot_services)
        return -2;

    if (g_uefi_context.boot_services->open_protocol &&
        g_uefi_context.boot_services->open_protocol(image_handle, (terra_efi_guid_t *)&g_loaded_image_guid, &loaded_image, image_handle, 0, TERRA_EFI_OPEN_PROTOCOL_GET_PROTOCOL) == TERRA_EFI_SUCCESS)
        g_uefi_context.loaded_image = (terra_efi_loaded_image_protocol_t *)loaded_image;

    protocols.read_file = uefi_read_file;
    protocols.read_blocks = uefi_read_blocks;
    protocols.get_memory_map = uefi_get_memory_map;
    protocols.get_gop = uefi_get_gop;
    protocols.read_key = uefi_read_key;
    protocols.get_acpi_rsdp = uefi_get_acpi_rsdp;
    protocols.exit_boot_services = uefi_exit_boot_services;
    protocols.context = &g_uefi_context;
    protocols.last_map_key = 0;
    terra_uefi_install_protocols(&protocols);
    return 0;
}
