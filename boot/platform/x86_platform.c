#include "x86_platform.h"

static terra_x86_platform_state_t g_x86_state;

extern void terra_arch_enter_kernel(uint64_t entry, const terra_boot_info_t *handoff);

static int strings_equal(const char *a, const char *b) {
    uint32_t i = 0;
    if (!a || !b)
        return 0;
    while (a[i] || b[i]) {
        if (a[i] != b[i])
            return 0;
        ++i;
    }
    return 1;
}

static void copy_bytes(uint64_t destination, uint64_t source, uint64_t length) {
    uint8_t *dst = (uint8_t *)(uintptr_t)destination;
    const uint8_t *src = (const uint8_t *)(uintptr_t)source;
    for (uint64_t i = 0; i < length; ++i)
        dst[i] = src[i];
}

static void copy_path(char *dst, const char *src) {
    uint32_t i = 0;
    if (!dst)
        return;
    if (!src)
        src = "";
    while (src[i] && i + 1 < TERRA_BOOT_MAX_STRING) {
        dst[i] = src[i];
        ++i;
    }
    dst[i] = '\0';
}

static void zero_state(void) {
    volatile uint8_t *bytes = (volatile uint8_t *)&g_x86_state;
    for (uint32_t i = 0; i < sizeof(g_x86_state); ++i)
        bytes[i] = 0;
}

void terra_x86_platform_reset(terra_x86_firmware_kind_t firmware) {
    zero_state();
    g_x86_state.firmware = firmware;
    g_x86_state.cpu_count = 1;
    g_x86_state.bsp_lapic_id = 0;
}

void terra_x86_platform_set_state(const terra_x86_platform_state_t *state) {
    if (!state)
        return;
    g_x86_state = *state;
    if (g_x86_state.cpu_count == 0)
        g_x86_state.cpu_count = 1;
}

terra_x86_platform_state_t *terra_x86_platform_state(void) {
    if (g_x86_state.cpu_count == 0)
        terra_x86_platform_reset(TERRA_X86_FIRMWARE_UNKNOWN);
    return &g_x86_state;
}

int terra_x86_platform_set_framebuffer(const terra_boot_framebuffer_t *framebuffer) {
    if (!framebuffer || !framebuffer->pixels || framebuffer->width == 0 || framebuffer->height == 0 || framebuffer->pitch_pixels == 0)
        return -1;
    terra_x86_platform_state()->framebuffer = *framebuffer;
    return 0;
}

int terra_x86_platform_add_memory_region(uint64_t base, uint64_t length, uint32_t type, uint32_t flags) {
    terra_x86_platform_state_t *state = terra_x86_platform_state();
    if (length == 0 || state->memory_map_count >= TERRA_BOOTLOADER_MAX_MEMMAP)
        return -1;
    state->memory_map[state->memory_map_count].base = base;
    state->memory_map[state->memory_map_count].length = length;
    state->memory_map[state->memory_map_count].type = type;
    state->memory_map[state->memory_map_count].flags = flags;
    ++state->memory_map_count;
    if (type == TERRA_MEMORY_USABLE)
        state->total_usable_memory += length;
    return 0;
}

void terra_x86_platform_set_rsdp(uint64_t rsdp) {
    terra_x86_platform_state()->rsdp = rsdp;
}

void terra_x86_platform_set_smp(uint32_t cpu_count, uint32_t bsp_lapic_id) {
    terra_x86_platform_state_t *state = terra_x86_platform_state();
    state->cpu_count = cpu_count ? cpu_count : 1;
    state->bsp_lapic_id = bsp_lapic_id;
}

void terra_x86_platform_set_file_loader(terra_x86_load_file_fn load_file, void *context) {
    terra_x86_platform_state_t *state = terra_x86_platform_state();
    state->load_file = load_file;
    state->context = context;
}

int terra_x86_platform_register_file(const char *path, uint64_t source, uint64_t length) {
    terra_x86_platform_state_t *state = terra_x86_platform_state();
    if (!path || !path[0] || source == 0 || length == 0)
        return -1;
    for (uint32_t i = 0; i < state->preloaded_file_count; ++i) {
        if (strings_equal(state->preloaded_files[i].path, path)) {
            state->preloaded_files[i].source = source;
            state->preloaded_files[i].length = length;
            return 0;
        }
    }
    if (state->preloaded_file_count >= TERRA_X86_PRELOADED_FILE_MAX)
        return -2;
    copy_path(state->preloaded_files[state->preloaded_file_count].path, path);
    state->preloaded_files[state->preloaded_file_count].source = source;
    state->preloaded_files[state->preloaded_file_count].length = length;
    ++state->preloaded_file_count;
    return 0;
}

void terra_x86_platform_set_key_reader(terra_x86_read_key_fn read_key, void *context) {
    terra_x86_platform_state_t *state = terra_x86_platform_state();
    state->read_key = read_key;
    state->context = context;
}

void terra_x86_platform_set_kernel_enter(terra_x86_enter_kernel_fn enter_kernel, void *context) {
    terra_x86_platform_state_t *state = terra_x86_platform_state();
    state->enter_kernel = enter_kernel;
    state->context = context;
}

int terra_x86_platform_get_framebuffer(terra_boot_framebuffer_t *framebuffer) {
    terra_x86_platform_state_t *state = terra_x86_platform_state();
    if (!framebuffer)
        return -1;
    *framebuffer = state->framebuffer;
    return framebuffer->pixels ? 0 : -1;
}

terra_boot_key_t terra_x86_platform_read_key(void *user) {
    terra_x86_platform_state_t *state = terra_x86_platform_state();
    if (state->read_key)
        return state->read_key(user ? user : state->context);
    return TERRA_BOOT_KEY_NONE;
}

int terra_x86_platform_load_file(const char *path, uint64_t destination, uint64_t *length) {
    terra_x86_platform_state_t *state = terra_x86_platform_state();
    if (!path || !path[0] || destination == 0)
        return -1;
    if (length)
        *length = 0;
    for (uint32_t i = 0; i < state->preloaded_file_count; ++i) {
        if (strings_equal(state->preloaded_files[i].path, path)) {
            copy_bytes(destination, state->preloaded_files[i].source, state->preloaded_files[i].length);
            if (length)
                *length = state->preloaded_files[i].length;
            return 0;
        }
    }
    if (!state->load_file)
        return -2;
    return state->load_file(state->context, path, destination, length);
}

int terra_x86_platform_get_memory_map(terra_boot_info_t *handoff) {
    terra_x86_platform_state_t *state = terra_x86_platform_state();
    if (!handoff || state->memory_map_count == 0)
        return -1;
    if (state->memory_map_count > TERRA_BOOTLOADER_MAX_MEMMAP)
        return -2;
    handoff->memory_map_count = state->memory_map_count;
    handoff->mmTotal = state->total_usable_memory;
    for (uint32_t i = 0; i < state->memory_map_count; ++i)
        handoff->memory_map[i] = state->memory_map[i];
    handoff->smp = &handoff->smp_storage;
    handoff->smp_storage.cpu_count = state->cpu_count ? state->cpu_count : 1;
    handoff->smp_storage.bsp_lapic_id = state->bsp_lapic_id;
    handoff->smpBspIndex = 0;
    return 0;
}

uint64_t terra_x86_platform_get_rsdp(void) {
    return terra_x86_platform_state()->rsdp;
}

uint64_t terra_x86_platform_get_device_tree(void) {
    return 0;
}

void terra_x86_platform_enter_kernel(uint64_t entry, const terra_boot_info_t *handoff) {
    terra_x86_platform_state_t *state = terra_x86_platform_state();
    if (state->enter_kernel)
        state->enter_kernel(state->context, entry, handoff);
    else
        terra_arch_enter_kernel(entry, handoff);
    for (;;)
        ;
}


