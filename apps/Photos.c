#pragma once

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
