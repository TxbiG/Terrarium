#pragma once

/*
    Display Photos and videos
*/

#include <stdinc.h>
#include <stdio.h>
#include <string.h>


#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <wayland-client.h>

struct wl_display* display = NULL;
struct wl_compositor* compositor = NULL;
struct wl_surface* surface = NULL;

typedef struct {
    char filepath[256];   // path to the image
    int width;
    int height;
    unsigned char* pixels; // RGBA buffer
} photo_t;

int load_photo(const char* path, photo_t* photo) {
    if (!photo) return -1;

    int channels;
    unsigned char* data = stbi_load(path, &photo->width, &photo->height, &channels, 4);
    if (!data) {
        fprintf(stderr, "Failed to load image: %s\n", path);
        return -1;
    }

    photo->pixels = data;
    strncpy(photo->filepath, path, sizeof(photo->filepath)-1);
    photo->filepath[sizeof(photo->filepath)-1] = '\0';
    return 0;
}

void free_photo(photo_t* photo) {
    if (photo && photo->pixels) {
        stbi_image_free(photo->pixels);
        photo->pixels = NULL;
    }
}

void render_init(void) {
    display = wl_display_connect(NULL);
    if (!display) {
        fprintf(stderr, "Failed to connect to Wayland\n");
        exit(1);
    }

    // TODO: get compositor from registry
    surface = wl_compositor_create_surface(compositor);
    if (!surface) {
        fprintf(stderr, "Failed to create surface\n");
        exit(1);
    }

    printf("Render initialized.\n");
}

void render_photo(photo_t* photo) {
    if (!photo || !photo->pixels) return;

    // TODO: draw RGBA pixels to SHM/EGL buffer
    printf("Rendering photo %s (%dx%d)\n", photo->filepath, photo->width, photo->height);

    // Wayland: attach buffer, commit, flush
    // wl_surface_attach(surface, buffer, 0, 0);
    // wl_surface_damage(surface, 0, 0, photo->width, photo->height);
    // wl_surface_commit(surface);
}

void render_shutdown(void) {
    if (surface) wl_surface_destroy(surface);
    if (display) wl_display_disconnect(display);
    printf("Render shutdown.\n");
}

int main(int argc, char** argv) {
    if (argc < 2) {
        printf("Usage: %s <image_path>\n", argv[0]);
        return 1;
    }

    photo_t photo = {0};

    if (load_photo(argv[1], &photo) != 0) {
        return 1;
    }

    render_init();
    render_photo(&photo);

    // Main loop (Wayland event handling)
    while (1) {
        // poll for events here, e.g., keyboard, close window
    }

    render_shutdown();
    free_photo(&photo);
    return 0;
}



typedef struct {
    char filepath[256];
    audio_stream_id_t stream_id;
    uint32_t sample_rate;
    uint8_t channels;
    audio_format_t format;
    float volume;
    int is_playing;
} media_t;

int load_media(const char* path, media_t* media) {
    if (!media || !path) return -1;
    strncpy(media->filepath, path, sizeof(media->filepath)-1);
    media->filepath[sizeof(media->filepath)-1] = '\0';
    media->volume = 1.0f;
    media->is_playing = 0;
    return 0;
}

int play_media(media_t* media) {
    if (!media) return -1;

    audio_stream_desc_t desc = {0};
    desc.device_id = 0; // default output device
    desc.direction = OS_AUDIO_OUTPUT;
    desc.format = AUDIO_FORMAT_F32LE;
    desc.channels = OS_AUDIO_CHANNEL_STEREO;
    desc.sample_rate = 44100;
    desc.buffer_frames = 512;
    desc.exclusive_mode = 0;

    int ret = os_audio_stream_create(&desc, NULL, NULL, &media->stream_id);
    if (ret != OS_AUDIO_OK) return ret;

    os_audio_stream_start(media->stream_id);
    media->is_playing = 1;

    printf("Playing %s\n", media->filepath);
    return 0;
}

int stop_media(media_t* media) {
    if (!media || !media->is_playing) return -1;
    os_audio_stream_stop(media->stream_id);
    os_audio_stream_destroy(media->stream_id);
    media->is_playing = 0;
    return 0;
}

int set_volume(media_t* media, float volume) {
    if (!media) return -1;
    media->volume = volume;
    os_audio_stream_set_volume(media->stream_id, volume);
    return 0;
}

struct wl_display* display = NULL;
struct wl_compositor* compositor = NULL;
struct wl_surface* surface = NULL;

void render_init(void) {
    display = wl_display_connect(NULL);
    if (!display) {
        fprintf(stderr, "Failed to connect to Wayland\n");
        return;
    }
    // TODO: get compositor and create surface
    printf("Render initialized.\n");
}

void render_media(media_t* media) {
    if (!media) return;

    printf("Rendering media: %s\n", media->filepath);
    printf("[GUI] Play/Pause, Volume, Seek bars here\n");

    // TODO: Draw GUI controls (buttons, slider, progress bar) via SHM/EGL
}

void render_shutdown(void) {
    if (surface) wl_surface_destroy(surface);
    if (display) wl_display_disconnect(display);
    printf("Render shutdown.\n");
}


int main(int argc, char** argv) {
    if (argc < 2) {
        printf("Usage: %s <audio_file>\n", argv[0]);
        return 1;
    }

    media_t media = {0};
    if (load_media(argv[1], &media) != 0) return 1;

    render_init();
    render_media(&media);

    if (play_media(&media) != 0) return 1;

    // Main loop for GUI events
    int running = 1;
    while (running) {
        // TODO: poll Wayland events
        // TODO: handle keyboard/mouse for play/pause, volume, seek
    }

    stop_media(&media);
    render_shutdown();
    return 0;
}
