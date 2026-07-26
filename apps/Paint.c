#pragma once
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <wayland-client.h>
#include <stdio.h>

typedef enum {
    TOOL_PENCIL,
    TOOL_ERASER,
    TOOL_LINE,
    TOOL_RECTANGLE,
    TOOL_FILL
} paint_tool_t;

typedef struct {
    int width;
    int height;
    uint32_t* pixels; // ARGB format
} canvas_t;

typedef struct {
    canvas_t canvas;
    paint_tool_t current_tool;
    uint32_t current_color;
    int mouse_down;
    int last_x;
    int last_y;
} paint_app_t;

void paint_init(paint_app_t* app, int width, int height) {
    if (!app) return;
    app->canvas.width = width;
    app->canvas.height = height;
    app->canvas.pixels = malloc(sizeof(uint32_t) * width * height);
    memset(app->canvas.pixels, 0xFFFFFFFF, width * height * sizeof(uint32_t)); // white background
    app->current_tool = TOOL_PENCIL;
    app->current_color = 0xFF000000; // black
    app->mouse_down = 0;
    app->last_x = 0;
    app->last_y = 0;
}

void paint_shutdown(paint_app_t* app) {
    if (!app) return;
    if (app->canvas.pixels) free(app->canvas.pixels);
}

void paint_set_tool(paint_app_t* app, paint_tool_t tool) {
    if (!app) return;
    app->current_tool = tool;
}

void paint_set_color(paint_app_t* app, uint32_t color) {
    if (!app) return;
    app->current_color = color;
}

void paint_draw(paint_app_t* app, int x, int y) {
    if (!app || x < 0 || y < 0 || x >= app->canvas.width || y >= app->canvas.height) return;

    switch (app->current_tool) {
        case TOOL_PENCIL:
            app->canvas.pixels[y * app->canvas.width + x] = app->current_color;
            break;
        case TOOL_ERASER:
            app->canvas.pixels[y * app->canvas.width + x] = 0xFFFFFFFF; // erase to white
            break;
        case TOOL_LINE:
        case TOOL_RECTANGLE:
        case TOOL_FILL:
            // TODO: implement advanced tools
            break;
    }

    app->last_x = x;
    app->last_y = y;
}

void paint_clear_canvas(paint_app_t* app) {
    if (!app) return;
    memset(app->canvas.pixels, 0xFFFFFFFF, app->canvas.width * app->canvas.height * sizeof(uint32_t));
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

void render_paint(paint_app_t* app) {
    if (!app) return;

    // TODO: draw app->canvas.pixels to Wayland surface
    // Draw GUI tools: pencil, eraser, color picker
    printf("Rendering paint canvas (%dx%d)\n", app->canvas.width, app->canvas.height);
}

void render_shutdown(void) {
    if (surface) wl_surface_destroy(surface);
    if (display) wl_display_disconnect(display);
    printf("Render shutdown.\n");
}



int main() {
    paint_app_t app;
    paint_init(&app, 800, 600);

    render_init();
    render_paint(&app);

    int running = 1;
    while (running) {
        // TODO: poll Wayland events
        // Handle mouse click & drag for drawing
        // Handle keyboard shortcuts for tool selection
        render_paint(&app);
    }

    paint_shutdown(&app);
    render_shutdown();
    return 0;
}
