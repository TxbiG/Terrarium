#ifndef TERRARIUM_UI_WINDOW_H
#define TERRARIUM_UI_WINDOW_H

#include "../../gfx.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum terra_window_flags {
    TERRA_WINDOW_NONE = 0,
    TERRA_WINDOW_NO_TITLEBAR = 1 << 0,
    TERRA_WINDOW_NO_RESIZE = 1 << 1,
    TERRA_WINDOW_NO_MOVE = 1 << 2,
    TERRA_WINDOW_NO_SCROLLBAR = 1 << 3,
    TERRA_WINDOW_ALWAYS_AUTO_RESIZE = 1 << 4,
    TERRA_WINDOW_NO_BACKGROUND = 1 << 5,
    TERRA_WINDOW_NO_INPUTS = 1 << 6,
    TERRA_WINDOW_MENU_BAR = 1 << 7,
    TERRA_WINDOW_UNSAVED_DOCUMENT = 1 << 8,
} terra_window_flags_t;

typedef enum terra_window_mode {
    TERRA_WINDOW_MODE_WINDOWED = 0,
    TERRA_WINDOW_MODE_MINIMIZED = 1,
    TERRA_WINDOW_MODE_MAXIMIZED = 2,
    TERRA_WINDOW_MODE_BORDERLESS = 3,
    TERRA_WINDOW_MODE_FULLSCREEN = 4,
} terra_window_mode_t;

typedef struct terra_window {
    unsigned id;
    const char *title;
    recti_t rect;
    terra_window_mode_t mode;
    unsigned flags;
    int visible;
    int resizable;
    int closeable;
    void *native_handle;
} terra_window_t;

terra_window_t terra_window_create(
    unsigned id,
    const char *title,
    int x,
    int y,
    int width,
    int height,
    terra_window_mode_t mode,
    unsigned flags);

void terra_window_destroy(terra_window_t *window);
void terra_window_set_title(terra_window_t *window, const char *title);
void terra_window_set_visible(terra_window_t *window, int visible);
void terra_window_set_mode(terra_window_t *window, terra_window_mode_t mode);
void terra_window_set_rect(terra_window_t *window, recti_t rect);
int terra_window_is_visible(const terra_window_t *window);

#ifdef __cplusplus
}
#endif

#endif
