#include "audio.h"

#include "../drivers.h"

typedef struct terra_audio_record {
    terra_audio_device_info_t info;
    terra_audio_write_fn_t write;
    terra_audio_read_fn_t read;
    void *context;
} terra_audio_record_t;

static terra_audio_record_t audio_devices[TERRA_AUDIO_MAX_DEVICES];
static terra_audio_id_t next_audio_id;
static int audio_ready;

static void audio_copy_name(char *dest, const char *src) {
    uint32_t i = 0;
    if (src) {
        for (; i + 1 < TERRA_AUDIO_NAME_MAX && src[i]; ++i)
            dest[i] = src[i];
    }
    dest[i] = 0;
}

static terra_audio_record_t *audio_find(terra_audio_id_t id) {
    for (uint32_t i = 0; i < TERRA_AUDIO_MAX_DEVICES; ++i) {
        if (audio_devices[i].info.active && audio_devices[i].info.id == id)
            return &audio_devices[i];
    }
    return 0;
}

static int audio_driver_init(const terra_boot_info_t *boot_info) {
    (void)boot_info;
    return audio_ready ? 0 : -1;
}

void audio_init(const terra_boot_info_t *boot_info) {
    (void)boot_info;
    for (uint32_t i = 0; i < TERRA_AUDIO_MAX_DEVICES; ++i)
        audio_devices[i].info.active = 0;
    next_audio_id = 1;
    audio_ready = 1;
    terra_driver_t driver = {"audio-core", TERRA_DRIVER_AUDIO, audio_driver_init, audio_shutdown, 1};
    drivers_register(&driver);
}

void audio_shutdown(void) {
    for (uint32_t i = 0; i < TERRA_AUDIO_MAX_DEVICES; ++i)
        audio_devices[i].info.active = 0;
    audio_ready = 0;
}

int audio_register_device(const terra_audio_device_config_t *config, terra_audio_id_t *out_id) {
    if (!audio_ready || !config || !config->name || !out_id)
        return -1;
    if (!config->write && !config->read)
        return -1;

    for (uint32_t i = 0; i < TERRA_AUDIO_MAX_DEVICES; ++i) {
        if (audio_devices[i].info.active)
            continue;
        audio_devices[i].info.id = next_audio_id++;
        audio_copy_name(audio_devices[i].info.name, config->name);
        audio_devices[i].info.direction = config->direction;
        audio_devices[i].info.sample_rate = config->sample_rate;
        audio_devices[i].info.channels = config->channels;
        audio_devices[i].info.bits_per_sample = config->bits_per_sample;
        audio_devices[i].info.bytes_written = 0;
        audio_devices[i].info.bytes_read = 0;
        audio_devices[i].info.active = 1;
        audio_devices[i].write = config->write;
        audio_devices[i].read = config->read;
        audio_devices[i].context = config->context;
        *out_id = audio_devices[i].info.id;
        return 0;
    }
    return -1;
}

int audio_unregister_device(terra_audio_id_t id) {
    terra_audio_record_t *record = audio_find(id);
    if (!record)
        return -1;
    record->info.active = 0;
    return 0;
}

int audio_info(terra_audio_id_t id, terra_audio_device_info_t *out_info) {
    terra_audio_record_t *record = audio_find(id);
    if (!record || !out_info)
        return -1;
    *out_info = record->info;
    return 0;
}

int audio_write(terra_audio_id_t id, const void *samples, size_t bytes) {
    terra_audio_record_t *record = audio_find(id);
    if (!record || !record->write || !samples || !bytes)
        return -1;
    int written = record->write(id, samples, bytes, record->context);
    if (written > 0)
        record->info.bytes_written += (uint64_t)written;
    return written;
}

int audio_read(terra_audio_id_t id, void *samples, size_t bytes) {
    terra_audio_record_t *record = audio_find(id);
    if (!record || !record->read || !samples || !bytes)
        return -1;
    int read = record->read(id, samples, bytes, record->context);
    if (read > 0)
        record->info.bytes_read += (uint64_t)read;
    return read;
}
