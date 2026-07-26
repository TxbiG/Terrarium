/*!
 * @file Reson.h
 * @brief Reson is the operating system's core audio interface, providing low-latency, real-time safe access to audio input and output devices.
 *
 * Reson Audio Library
 * ===================
 *
 * It abstracts audio hardware into devices and streams, enabling
 * applications to capture and render audio through a unified,
 * callback-driven API.
 *
 * Reson is designed as a thin user-space layer over the kernel audio
 * subsystem. All policy, mixing, routing, and format conversion are
 * handled outside the kernel, while the kernel is responsible only for
 * reliable and efficient sample transport.
 *
 * Key characteristics:
 *  - Real-time safe, non-blocking audio callbacks
 *  - Shared-memory-based audio transport
 *  - Support for shared and exclusive device access
 *  - Explicit control over latency and buffering
 *
 * Reson does not manage application permissions, UI policy, or media
 * session control; these concerns are handled by a higher-level system
 * services.
 *
 * This header defines the stable public API and ABI contract between
 * applications and the operating system audio stack.
 */

#ifndef TERRARIUM_RESON_H
#define TERRARIUM_RESON_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>


#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

typedef uint32_t audio_device_id_t;
typedef uint32_t audio_stream_id_t;
typedef uint64_t audio_time_t; /* nanoseconds */

typedef enum {
    AUDIO_FORMAT_UNKNOWN   = 0x0000u,  // Unspecified audio format
    AUDIO_FORMAT_U8        = 0x0008u,  // Unsigned 8-bit samples
    AUDIO_FORMAT_S8        = 0x8008u,  // Signed 8-bit samples
    AUDIO_FORMAT_S16LE     = 0x8010u,  // Signed 16-bit samples
    AUDIO_FORMAT_S16BE     = 0x9010u,  // As above, but big-endian byte order
    AUDIO_FORMAT_S32LE     = 0x8020u,  // 32-bit integer samples
    AUDIO_FORMAT_S32BE     = 0x9020u,  // As above, but big-endian byte order
    AUDIO_FORMAT_F32LE     = 0x8120u,  // 32-bit floating point samples
    AUDIO_FORMAT_F32BE     = 0x9120u,  // As above, but big-endian byte order
} audio_format_t;

typedef enum {
    OS_AUDIO_CHANNEL_MONO,   = 1,
    OS_AUDIO_CHANNEL_STEREO, = 2,
    OS_AUDIO_CHANNEL_4_1,    = 6,
    OS_AUDIO_CHANNEL_5_1,    = 6,
    OS_AUDIO_CHANNEL_6_1,    = 6,
    OS_AUDIO_CHANNEL_7_1,    = 8
    OS_AUDIO_CHANNEL_SPATIAL = 8
} audio_channel_layout_t;

typedef enum {
    AUDIO_INPUT,
    AUDIO_OUTPUT
} audio_direction_t;

typedef struct {
    audio_format_t format;            // Audio data format
    audio_channel_layout_t channels;  // Number of channels
    int freq;                         //sample rate: sample frames per second
} audio_spec_t;

typedef struct {
    audio_device_id_t id;
    char name[64];
    audio_direction_t direction;
    uint32_t sample_rate;
    audio_channel_layout_t channels;
    audio_format_t format;
    uint32_t min_buffer_frames;
    uint32_t max_buffer_frames;
} audio_device_info_t;

/* ===============================
   Stream Configuration
   =============================== */

typedef struct {
    audio_device_id_t device_id;
    audio_direction_t direction;
    audio_format_t format;
    audio_channel_layout_t channels;
    uint32_t sample_rate;
    uint32_t buffer_frames;
    uint8_t exclusive_mode; /* 0 = shared, 1 = exclusive */
} audio_stream_desc_t;

/* ===============================
   Audio Buffers
   =============================== */

typedef struct {
    void *data;
    size_t frames;
    audio_time_t timestamp;
} audio_buffer_t;

/* ===============================
   Callback Interface
   =============================== */

typedef void (*audio_callback_t)(audio_stream_id_t stream_id, audio_buffer_t *buffer, void *user_data);

/* ===============================
   Core API
   =============================== */

/* Library lifecycle */
int os_audio_init(void);
void os_audio_shutdown(void);

/* Device enumeration */
int os_audio_get_device_count(void);
int os_audio_get_device_info(
    uint32_t index,
    os_audio_device_info_t *out_info
);

/* Stream management */
int os_audio_stream_create(
    const os_audio_stream_desc_t *desc,
    audio_callback_t callback,
    void *user_data,
    os_audio_stream_id_t *out_stream_id
);

int os_audio_stream_start(oaudio_stream_id_t stream_id);
int os_audio_stream_stop(audio_stream_id_t stream_id);
int os_audio_stream_destroy(os_audio_stream_id_t stream_id);

/* ===============================
   Stream Control
   =============================== */

int os_audio_stream_set_volume(
    audio_stream_id_t stream_id,
    float volume /* 0.0 – 1.0 */
);

int os_audio_stream_get_latency(
    audio_stream_id_t stream_id,
    uint32_t *out_latency_frames
);

/* ===============================
   Global Control
   =============================== */

int os_audio_set_master_volume(float volume);
int os_audio_get_master_volume(float *out_volume);

/* ===============================
   Timing
   =============================== */

os_audio_time_t os_audio_get_time(void);

/* ===============================
   Error Codes
   =============================== */

#define OS_AUDIO_OK                  0
#define OS_AUDIO_ERR_INVALID        -1
#define OS_AUDIO_ERR_NO_DEVICE      -2
#define OS_AUDIO_ERR_UNSUPPORTED    -3
#define OS_AUDIO_ERR_BUSY           -4
#define OS_AUDIO_ERR_NO_MEMORY      -5
#define OS_AUDIO_ERR_INTERNAL       -6


/*
effects:
audio_effect_chain_t;
audio_stream_attach_effect();

Allowed format?:
int audio_stream_negotiate_format(...);

*/

#ifdef __cplusplus
}
#endif  // __cplusplus

#endif // TERRARIUM_RESON_H
