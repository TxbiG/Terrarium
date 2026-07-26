#ifndef TERRARIUM_DRIVERS_GPU_H
#define TERRARIUM_DRIVERS_GPU_H

#include <bootloader.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define TERRA_GPU_MAX_DEVICES 16u
#define TERRA_GPU_NAME_MAX 32u
#define TERRA_GPU_MAX_BUFFERS 128u
#define TERRA_GPU_MAX_FENCES 128u
#define TERRA_GPU_ABI_VERSION 1u

typedef uint32_t terra_gpu_id_t;
typedef uint32_t terra_gpu_buffer_id_t;
typedef uint32_t terra_gpu_fence_id_t;
typedef int (*terra_gpu_present_fn_t)(terra_gpu_id_t id, const void *pixels, size_t bytes, void *context);

typedef enum terra_gpu_device_kind {
    TERRA_GPU_DEVICE_FRAMEBUFFER = 0,
    TERRA_GPU_DEVICE_SOFTWARE_RENDER,
    TERRA_GPU_DEVICE_RENDER_NODE
} terra_gpu_device_kind_t;

typedef enum terra_gpu_buffer_flags {
    TERRA_GPU_BUFFER_CPU_READ = 1u << 0,
    TERRA_GPU_BUFFER_CPU_WRITE = 1u << 1,
    TERRA_GPU_BUFFER_GPU_READ = 1u << 2,
    TERRA_GPU_BUFFER_GPU_WRITE = 1u << 3,
    TERRA_GPU_BUFFER_SCANOUT = 1u << 4,
    TERRA_GPU_BUFFER_SHARED = 1u << 5
} terra_gpu_buffer_flags_t;

typedef enum terra_gpu_cache_policy {
    TERRA_GPU_CACHE_DEFAULT = 0,
    TERRA_GPU_CACHE_WRITE_COMBINED,
    TERRA_GPU_CACHE_COHERENT,
    TERRA_GPU_CACHE_UNCACHED
} terra_gpu_cache_policy_t;

typedef struct terra_gpu_config {
    const char *name;
    uint64_t framebuffer;
    uint32_t width;
    uint32_t height;
    uint32_t pitch;
    uint16_t bpp;
    terra_gpu_present_fn_t present;
    void *context;
} terra_gpu_config_t;

typedef struct terra_gpu_info {
    terra_gpu_id_t id;
    char name[TERRA_GPU_NAME_MAX];
    terra_gpu_device_kind_t kind;
    uint32_t abi_version;
    uint64_t framebuffer;
    uint32_t width;
    uint32_t height;
    uint32_t pitch;
    uint16_t bpp;
    uint64_t frames_presented;
    uint32_t supports_buffers;
    uint32_t supports_command_submission;
    uint32_t supports_fences;
    int active;
} terra_gpu_info_t;

typedef struct terra_gpu_buffer_desc {
    uint64_t size;
    uint32_t width;
    uint32_t height;
    uint32_t stride;
    uint32_t format;
    uint32_t flags;
    terra_gpu_cache_policy_t cache_policy;
} terra_gpu_buffer_desc_t;

typedef struct terra_gpu_buffer_info {
    terra_gpu_buffer_id_t id;
    terra_gpu_id_t owner;
    terra_gpu_buffer_desc_t desc;
    uint64_t mmap_offset;
    uint32_t exported_handle;
    int active;
} terra_gpu_buffer_info_t;

typedef struct terra_gpu_submit_desc {
    terra_gpu_id_t device;
    terra_gpu_buffer_id_t command_buffer;
    const terra_gpu_buffer_id_t *buffers;
    size_t buffer_count;
    const terra_gpu_fence_id_t *wait_fences;
    size_t wait_fence_count;
    terra_gpu_fence_id_t signal_fence;
    uint32_t flags;
} terra_gpu_submit_desc_t;

void gpu_init(const terra_boot_info_t *boot_info);
void gpu_shutdown(void);
int gpu_register_device(const terra_gpu_config_t *config, terra_gpu_id_t *out_id);
int gpu_unregister_device(terra_gpu_id_t id);
int gpu_info(terra_gpu_id_t id, terra_gpu_info_t *out_info);
int gpu_present(terra_gpu_id_t id, const void *pixels, size_t bytes);

int gpu_render_device_count(void);
int gpu_render_device_info(uint32_t index, terra_gpu_info_t *out_info);
int gpu_buffer_create(terra_gpu_id_t owner, const terra_gpu_buffer_desc_t *desc, terra_gpu_buffer_id_t *out_id);
int gpu_buffer_info(terra_gpu_buffer_id_t id, terra_gpu_buffer_info_t *out_info);
int gpu_buffer_export(terra_gpu_buffer_id_t id, uint32_t *out_handle);
int gpu_buffer_import(terra_gpu_id_t owner, uint32_t exported_handle, terra_gpu_buffer_id_t *out_id);
int gpu_buffer_mmap(terra_gpu_buffer_id_t id, uint64_t *out_offset, uint64_t *out_size, terra_gpu_cache_policy_t *out_cache_policy);
int gpu_buffer_destroy(terra_gpu_buffer_id_t id);
int gpu_fence_create(terra_gpu_fence_id_t *out_id);
int gpu_fence_signal(terra_gpu_fence_id_t id);
int gpu_fence_wait(terra_gpu_fence_id_t id, uint64_t timeout_ns);
int gpu_submit(const terra_gpu_submit_desc_t *submit);

#ifdef __cplusplus
}
#endif

#endif
