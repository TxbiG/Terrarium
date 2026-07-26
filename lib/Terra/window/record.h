#ifndef LIBRECORDER_H
#define LIBRECORDER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/* Recording formats */
typedef enum {
    RECORD_FORMAT_UNKNOWN = 0,
    RECORD_FORMAT_MP4,
    RECORD_FORMAT_WEBM,
    RECORD_FORMAT_AVI
} record_format_t;

/* Encoder options */
typedef struct {
    int width;
    int height;
    int framerate;
    int bitrate_kbps;
    int enable_audio;
} recorder_config_t;

/* Recorder handle */
typedef struct recorder_t recorder_t;

/* Recorder API */
int recorder_init(void);
int recorder_shutdown(void);

/* Start / stop recording */
recorder_t* recorder_start(const char *output_file, record_format_t format, const recorder_config_t *config);
int recorder_capture_frame(recorder_t *rec, const void *frame_buffer, int stride);
int recorder_stop(recorder_t *rec);

/* Optional audio integration */
int recorder_add_audio(recorder_t *rec, const void *audio_buffer, int samples);

#ifdef __cplusplus
}
#endif

#endif /* LIBRECORDER_H */
