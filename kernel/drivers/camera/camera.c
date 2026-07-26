#include "camera.h"

#include "../drivers.h"

typedef struct terra_camera_record {
    terra_camera_info_t info;
    terra_camera_capture_fn_t capture;
    void *context;
} terra_camera_record_t;

static terra_camera_record_t cameras[TERRA_CAMERA_MAX_DEVICES];
static terra_camera_id_t next_camera_id;
static int camera_ready;

static void camera_copy_name(char *dest, const char *src) {
    uint32_t i = 0;
    if (src) {
        for (; i + 1 < TERRA_CAMERA_NAME_MAX && src[i]; ++i)
            dest[i] = src[i];
    }
    dest[i] = 0;
}

static terra_camera_record_t *camera_find(terra_camera_id_t id) {
    for (uint32_t i = 0; i < TERRA_CAMERA_MAX_DEVICES; ++i) {
        if (cameras[i].info.active && cameras[i].info.id == id)
            return &cameras[i];
    }
    return 0;
}

static int camera_driver_init(const terra_boot_info_t *boot_info) {
    (void)boot_info;
    return camera_ready ? 0 : -1;
}

void camera_init(const terra_boot_info_t *boot_info) {
    (void)boot_info;
    for (uint32_t i = 0; i < TERRA_CAMERA_MAX_DEVICES; ++i)
        cameras[i].info.active = 0;
    next_camera_id = 1;
    camera_ready = 1;
    terra_driver_t driver = {"camera-core", TERRA_DRIVER_MISC, camera_driver_init, camera_shutdown, 1};
    drivers_register(&driver);
}

void camera_shutdown(void) {
    for (uint32_t i = 0; i < TERRA_CAMERA_MAX_DEVICES; ++i)
        cameras[i].info.active = 0;
    camera_ready = 0;
}

int camera_register_device(const terra_camera_config_t *config, terra_camera_id_t *out_id) {
    if (!camera_ready || !config || !config->name || !config->capture || !out_id)
        return -1;
    for (uint32_t i = 0; i < TERRA_CAMERA_MAX_DEVICES; ++i) {
        if (cameras[i].info.active)
            continue;
        cameras[i].info.id = next_camera_id++;
        camera_copy_name(cameras[i].info.name, config->name);
        cameras[i].info.width = config->width;
        cameras[i].info.height = config->height;
        cameras[i].info.pixel_format = config->pixel_format;
        cameras[i].info.frames_captured = 0;
        cameras[i].info.active = 1;
        cameras[i].capture = config->capture;
        cameras[i].context = config->context;
        *out_id = cameras[i].info.id;
        return 0;
    }
    return -1;
}

int camera_unregister_device(terra_camera_id_t id) {
    terra_camera_record_t *record = camera_find(id);
    if (!record)
        return -1;
    record->info.active = 0;
    return 0;
}

int camera_info(terra_camera_id_t id, terra_camera_info_t *out_info) {
    terra_camera_record_t *record = camera_find(id);
    if (!record || !out_info)
        return -1;
    *out_info = record->info;
    return 0;
}

int camera_capture(terra_camera_id_t id, void *buffer, size_t bytes) {
    terra_camera_record_t *record = camera_find(id);
    if (!record || !buffer || !bytes)
        return -1;
    int result = record->capture(id, buffer, bytes, record->context);
    if (result >= 0)
        record->info.frames_captured++;
    return result;
}
