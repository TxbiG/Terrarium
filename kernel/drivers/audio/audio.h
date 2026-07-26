#ifndef TERRARIUM_DRIVERS_AUDIO_H
#define TERRARIUM_DRIVERS_AUDIO_H

#include <bootloader.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define TERRA_AUDIO_MAX_DEVICES 16u
#define TERRA_AUDIO_NAME_MAX 32u

typedef uint32_t terra_audio_id_t;

typedef enum terra_audio_direction {
    TERRA_AUDIO_OUTPUT = 1u,
    TERRA_AUDIO_INPUT = 2u,
    TERRA_AUDIO_DUPLEX = 3u
} terra_audio_direction_t;

typedef int (*terra_audio_write_fn_t)(terra_audio_id_t id, const void *samples, size_t bytes, void *context);
typedef int (*terra_audio_read_fn_t)(terra_audio_id_t id, void *samples, size_t bytes, void *context);

typedef struct terra_audio_device_config {
    const char *name;
    terra_audio_direction_t direction;
    uint32_t sample_rate;
    uint16_t channels;
    uint16_t bits_per_sample;
    terra_audio_write_fn_t write;
    terra_audio_read_fn_t read;
    void *context;
} terra_audio_device_config_t;

typedef struct terra_audio_device_info {
    terra_audio_id_t id;
    char name[TERRA_AUDIO_NAME_MAX];
    terra_audio_direction_t direction;
    uint32_t sample_rate;
    uint16_t channels;
    uint16_t bits_per_sample;
    uint64_t bytes_written;
    uint64_t bytes_read;
    int active;
} terra_audio_device_info_t;

void audio_init(const terra_boot_info_t *boot_info);
void audio_shutdown(void);
int audio_register_device(const terra_audio_device_config_t *config, terra_audio_id_t *out_id);
int audio_unregister_device(terra_audio_id_t id);
int audio_info(terra_audio_id_t id, terra_audio_device_info_t *out_info);
int audio_write(terra_audio_id_t id, const void *samples, size_t bytes);
int audio_read(terra_audio_id_t id, void *samples, size_t bytes);

#ifdef __cplusplus
}
#endif

#endif
