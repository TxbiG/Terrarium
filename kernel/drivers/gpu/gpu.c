#include "gpu.h"

#include "../drivers.h"

typedef struct terra_gpu_record {
    terra_gpu_info_t info;
    terra_gpu_present_fn_t present;
    void *context;
} terra_gpu_record_t;

typedef struct terra_gpu_buffer_record {
    terra_gpu_buffer_info_t info;
} terra_gpu_buffer_record_t;

typedef struct terra_gpu_fence_record {
    terra_gpu_fence_id_t id;
    int active;
    int signaled;
} terra_gpu_fence_record_t;

static terra_gpu_record_t gpus[TERRA_GPU_MAX_DEVICES];
static terra_gpu_buffer_record_t buffers[TERRA_GPU_MAX_BUFFERS];
static terra_gpu_fence_record_t fences[TERRA_GPU_MAX_FENCES];
static terra_gpu_id_t next_gpu_id;
static terra_gpu_buffer_id_t next_buffer_id;
static terra_gpu_fence_id_t next_fence_id;
static int gpu_ready;

static void gpu_copy_name(char *dest, const char *src) {
    uint32_t i = 0;
    if (src) {
        for (; i + 1 < TERRA_GPU_NAME_MAX && src[i]; ++i)
            dest[i] = src[i];
    }
    dest[i] = 0;
}

static terra_gpu_record_t *gpu_find(terra_gpu_id_t id) {
    for (uint32_t i = 0; i < TERRA_GPU_MAX_DEVICES; ++i) {
        if (gpus[i].info.active && gpus[i].info.id == id)
            return &gpus[i];
    }
    return 0;
}

static terra_gpu_buffer_record_t *gpu_buffer_find(terra_gpu_buffer_id_t id) {
    for (uint32_t i = 0; i < TERRA_GPU_MAX_BUFFERS; ++i) {
        if (buffers[i].info.active && buffers[i].info.id == id)
            return &buffers[i];
    }
    return 0;
}

static terra_gpu_fence_record_t *gpu_fence_find(terra_gpu_fence_id_t id) {
    for (uint32_t i = 0; i < TERRA_GPU_MAX_FENCES; ++i) {
        if (fences[i].active && fences[i].id == id)
            return &fences[i];
    }
    return 0;
}

static int gpu_driver_init(const terra_boot_info_t *boot_info) {
    (void)boot_info;
    return gpu_ready ? 0 : -1;
}

void gpu_init(const terra_boot_info_t *boot_info) {
    for (uint32_t i = 0; i < TERRA_GPU_MAX_DEVICES; ++i)
        gpus[i].info.active = 0;
    for (uint32_t i = 0; i < TERRA_GPU_MAX_BUFFERS; ++i)
        buffers[i].info.active = 0;
    for (uint32_t i = 0; i < TERRA_GPU_MAX_FENCES; ++i)
        fences[i].active = 0;

    next_gpu_id = 1;
    next_buffer_id = 1;
    next_fence_id = 1;
    gpu_ready = 1;

    if (boot_info && boot_info->framebuffer.address) {
        terra_gpu_config_t fb = {
            .name = "boot-framebuffer",
            .framebuffer = boot_info->framebuffer.address,
            .width = boot_info->framebuffer.width,
            .height = boot_info->framebuffer.height,
            .pitch = boot_info->framebuffer.pitch,
            .bpp = boot_info->framebuffer.bpp,
            .present = 0,
            .context = 0,
        };
        terra_gpu_id_t ignored = 0;
        gpu_register_device(&fb, &ignored);
    }

    terra_driver_t driver = {"gpu-core", TERRA_DRIVER_DISPLAY, gpu_driver_init, gpu_shutdown, 1};
    drivers_register(&driver);
}

void gpu_shutdown(void) {
    for (uint32_t i = 0; i < TERRA_GPU_MAX_DEVICES; ++i)
        gpus[i].info.active = 0;
    for (uint32_t i = 0; i < TERRA_GPU_MAX_BUFFERS; ++i)
        buffers[i].info.active = 0;
    for (uint32_t i = 0; i < TERRA_GPU_MAX_FENCES; ++i)
        fences[i].active = 0;
    gpu_ready = 0;
}

int gpu_register_device(const terra_gpu_config_t *config, terra_gpu_id_t *out_id) {
    if (!gpu_ready || !config || !config->name || !out_id)
        return -1;
    for (uint32_t i = 0; i < TERRA_GPU_MAX_DEVICES; ++i) {
        if (gpus[i].info.active)
            continue;
        gpus[i].info.id = next_gpu_id++;
        gpu_copy_name(gpus[i].info.name, config->name);
        gpus[i].info.kind = config->framebuffer ? TERRA_GPU_DEVICE_FRAMEBUFFER : TERRA_GPU_DEVICE_SOFTWARE_RENDER;
        gpus[i].info.abi_version = TERRA_GPU_ABI_VERSION;
        gpus[i].info.framebuffer = config->framebuffer;
        gpus[i].info.width = config->width;
        gpus[i].info.height = config->height;
        gpus[i].info.pitch = config->pitch;
        gpus[i].info.bpp = config->bpp;
        gpus[i].info.frames_presented = 0;
        gpus[i].info.supports_buffers = 1;
        gpus[i].info.supports_command_submission = 0;
        gpus[i].info.supports_fences = 1;
        gpus[i].info.active = 1;
        gpus[i].present = config->present;
        gpus[i].context = config->context;
        *out_id = gpus[i].info.id;
        return 0;
    }
    return -1;
}

int gpu_unregister_device(terra_gpu_id_t id) {
    terra_gpu_record_t *record = gpu_find(id);
    if (!record)
        return -1;
    record->info.active = 0;
    return 0;
}

int gpu_info(terra_gpu_id_t id, terra_gpu_info_t *out_info) {
    terra_gpu_record_t *record = gpu_find(id);
    if (!record || !out_info)
        return -1;
    *out_info = record->info;
    return 0;
}

int gpu_present(terra_gpu_id_t id, const void *pixels, size_t bytes) {
    terra_gpu_record_t *record = gpu_find(id);
    if (!record || !pixels || !bytes)
        return -1;
    if (record->present) {
        int result = record->present(id, pixels, bytes, record->context);
        if (result < 0)
            return result;
    }
    record->info.frames_presented++;
    return 0;
}

int gpu_render_device_count(void) {
    int count = 0;
    for (uint32_t i = 0; i < TERRA_GPU_MAX_DEVICES; ++i) {
        if (gpus[i].info.active)
            count++;
    }
    return count;
}

int gpu_render_device_info(uint32_t index, terra_gpu_info_t *out_info) {
    uint32_t seen = 0;
    if (!out_info)
        return -1;

    for (uint32_t i = 0; i < TERRA_GPU_MAX_DEVICES; ++i) {
        if (!gpus[i].info.active)
            continue;
        if (seen == index) {
            *out_info = gpus[i].info;
            return 0;
        }
        seen++;
    }

    return -1;
}

int gpu_buffer_create(terra_gpu_id_t owner, const terra_gpu_buffer_desc_t *desc, terra_gpu_buffer_id_t *out_id) {
    if (!gpu_ready || !gpu_find(owner) || !desc || !out_id || desc->size == 0)
        return -1;

    for (uint32_t i = 0; i < TERRA_GPU_MAX_BUFFERS; ++i) {
        if (buffers[i].info.active)
            continue;
        buffers[i].info.id = next_buffer_id++;
        buffers[i].info.owner = owner;
        buffers[i].info.desc = *desc;
        buffers[i].info.mmap_offset = 0;
        buffers[i].info.exported_handle = buffers[i].info.id;
        buffers[i].info.active = 1;
        *out_id = buffers[i].info.id;
        return 0;
    }

    return -1;
}

int gpu_buffer_info(terra_gpu_buffer_id_t id, terra_gpu_buffer_info_t *out_info) {
    terra_gpu_buffer_record_t *record = gpu_buffer_find(id);
    if (!record || !out_info)
        return -1;
    *out_info = record->info;
    return 0;
}

int gpu_buffer_export(terra_gpu_buffer_id_t id, uint32_t *out_handle) {
    terra_gpu_buffer_record_t *record = gpu_buffer_find(id);
    if (!record || !out_handle || !(record->info.desc.flags & TERRA_GPU_BUFFER_SHARED))
        return -1;
    *out_handle = record->info.exported_handle;
    return 0;
}

int gpu_buffer_import(terra_gpu_id_t owner, uint32_t exported_handle, terra_gpu_buffer_id_t *out_id) {
    if (!gpu_find(owner) || !out_id || exported_handle == 0)
        return -1;

    for (uint32_t i = 0; i < TERRA_GPU_MAX_BUFFERS; ++i) {
        if (!buffers[i].info.active || buffers[i].info.exported_handle != exported_handle)
            continue;
        *out_id = buffers[i].info.id;
        return 0;
    }

    return -1;
}

int gpu_buffer_mmap(terra_gpu_buffer_id_t id, uint64_t *out_offset, uint64_t *out_size, terra_gpu_cache_policy_t *out_cache_policy) {
    terra_gpu_buffer_record_t *record = gpu_buffer_find(id);
    if (!record || !out_offset || !out_size || !out_cache_policy)
        return -1;

    *out_offset = record->info.mmap_offset;
    *out_size = record->info.desc.size;
    *out_cache_policy = record->info.desc.cache_policy;
    return 0;
}

int gpu_buffer_destroy(terra_gpu_buffer_id_t id) {
    terra_gpu_buffer_record_t *record = gpu_buffer_find(id);
    if (!record)
        return -1;
    record->info.active = 0;
    return 0;
}

int gpu_fence_create(terra_gpu_fence_id_t *out_id) {
    if (!gpu_ready || !out_id)
        return -1;

    for (uint32_t i = 0; i < TERRA_GPU_MAX_FENCES; ++i) {
        if (fences[i].active)
            continue;
        fences[i].id = next_fence_id++;
        fences[i].active = 1;
        fences[i].signaled = 0;
        *out_id = fences[i].id;
        return 0;
    }

    return -1;
}

int gpu_fence_signal(terra_gpu_fence_id_t id) {
    terra_gpu_fence_record_t *record = gpu_fence_find(id);
    if (!record)
        return -1;
    record->signaled = 1;
    return 0;
}

int gpu_fence_wait(terra_gpu_fence_id_t id, uint64_t timeout_ns) {
    terra_gpu_fence_record_t *record = gpu_fence_find(id);
    (void)timeout_ns;
    if (!record)
        return -1;
    return record->signaled ? 0 : -1;
}

int gpu_submit(const terra_gpu_submit_desc_t *submit) {
    terra_gpu_record_t *device;

    if (!submit)
        return -1;
    device = gpu_find(submit->device);
    if (!device || !device->info.supports_command_submission)
        return -1;

    if (submit->signal_fence)
        gpu_fence_signal(submit->signal_fence);
    return 0;
}
