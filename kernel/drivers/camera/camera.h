#ifndef TERRARIUM_DRIVERS_CAMERA_H
#define TERRARIUM_DRIVERS_CAMERA_H

#include <bootloader.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define TERRA_CAMERA_MAX_DEVICES 16u
#define TERRA_CAMERA_NAME_MAX 32u

typedef uint32_t terra_camera_id_t;
typedef int (*terra_camera_capture_fn_t)(terra_camera_id_t id, void *buffer, size_t bytes, void *context);

typedef struct terra_camera_config {
    const char *name;
    uint32_t width;
    uint32_t height;
    uint32_t pixel_format;
    terra_camera_capture_fn_t capture;
    void *context;
} terra_camera_config_t;

typedef struct terra_camera_info {
    terra_camera_id_t id;
    char name[TERRA_CAMERA_NAME_MAX];
    uint32_t width;
    uint32_t height;
    uint32_t pixel_format;
    uint64_t frames_captured;
    int active;
} terra_camera_info_t;

void camera_init(const terra_boot_info_t *boot_info);
void camera_shutdown(void);
int camera_register_device(const terra_camera_config_t *config, terra_camera_id_t *out_id);
int camera_unregister_device(terra_camera_id_t id);
int camera_info(terra_camera_id_t id, terra_camera_info_t *out_info);
int camera_capture(terra_camera_id_t id, void *buffer, size_t bytes);

#ifdef __cplusplus
}
#endif

#endif
