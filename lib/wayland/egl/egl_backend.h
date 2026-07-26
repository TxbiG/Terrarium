#ifndef FLORA_EGL_BACKEND_H
#define FLORA_EGL_BACKEND_H

#include <stdint.h>
//#include "egl_core.h"

#ifdef  __cplusplus
extern "C" {
#endif

typedef struct surface;

struct egl_window
{
    const intptr_t version;

	int width;
	int height;
	int dx;
	int dy;

	int attached_width;
	int attached_height;

	void* driver_private;
	void (*resize_callback)(struct egl_window*, void*);
	void (*destroy_window_callback)(void*);

	struct surface* surface;
};

#ifdef  __cplusplus
}
#endif

#endif // FLORA_EGL_BACKEND_H
