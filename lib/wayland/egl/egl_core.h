#ifndef FLORA_EGL_CORE_H
#define FLORA_EGL_CORE_H

#ifdef  __cplusplus
extern "C" {
#endif

struct egl_window;
struct wl_surface;

struct egl_window* egl_window_create(struct wl_surface* surface, int width, int height);

void egl_window_destroy(struct egl_window* egl_window);

void egl_window_resize(struct egl_window* egl_window, int width, int height, int dx, int dy);

void egl_window_get_attached_size(struct egl_window* egl_window, int *width, int *height);

#ifdef  __cplusplus
}
#endif

#endif // FLORA_EGL_CORE_H