#include <stdlib.h>
#include <string.h>

#include "comp_egl.h"
#include "egl_backend.h"
#include "../src/util.h"



WL_EXPORT void egl_window_resize(struct egl_window* egl_window, int width, int height, int dx, int dy) {
	if (width <= 0 || height <= 0) { return; }

	egl_window->width  = width;
	egl_window->height = height;
	egl_window->dx     = dx;
	egl_window->dy     = dy;

	if (egl_window->resize_callback) { egl_window->resize_callback(egl_window, egl_window->driver_private); }
}

WL_EXPORT struct egl_window* egl_window_create(struct wl_surface *surface, int width, int height)
{
	struct egl_window* egl_window;

	if (width <= 0 || height <= 0) { return NULL; }

	egl_window = calloc(1, sizeof *egl_window);
	if (!egl_window) { return NULL; }

	intptr_t *version = (intptr_t *)&egl_window->version;
	*version = WL_EGL_WINDOW_VERSION;

	egl_window->surface = surface;
	egl_window->width  = width;
	egl_window->height = height;

	return egl_window;
}

WL_EXPORT void egl_window_destroy(struct egl_window* egl_window) {
	if (egl_window->destroy_window_callback) { egl_window->destroy_window_callback(egl_window->driver_private); }
	free(egl_window);
}

WL_EXPORT void egl_window_get_attached_size(struct egl_window* egl_window, int *width, int *height) {
	if (width) { *width = egl_window->attached_width; }
    if (height) { *height = egl_window->attached_height; }
}
