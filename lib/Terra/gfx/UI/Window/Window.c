#include "Window.h"

terra_window_t terra_window_create(
    unsigned id,
    const char *title,
    int x,
    int y,
    int width,
    int height,
    terra_window_mode_t mode,
    unsigned flags) {
    terra_window_t window;
    window.id = id;
    window.title = title;
    window.rect.x = x;
    window.rect.y = y;
    window.rect.width = width;
    window.rect.height = height;
    window.mode = mode;
    window.flags = flags;
    window.visible = 1;
    window.resizable = (flags & TERRA_WINDOW_NO_RESIZE) == 0;
    window.closeable = 1;
    window.native_handle = 0;
    return window;
}

void terra_window_destroy(terra_window_t *window) {
    if (!window)
        return;

    window->visible = 0;
    window->native_handle = 0;
}

void terra_window_set_title(terra_window_t *window, const char *title) {
    if (window)
        window->title = title;
}

void terra_window_set_visible(terra_window_t *window, int visible) {
    if (window)
        window->visible = visible ? 1 : 0;
}

void terra_window_set_mode(terra_window_t *window, terra_window_mode_t mode) {
    if (window)
        window->mode = mode;
}

void terra_window_set_rect(terra_window_t *window, recti_t rect) {
    if (window)
        window->rect = rect;
}

int terra_window_is_visible(const terra_window_t *window) {
    return window ? window->visible : 0;
}
