#ifndef TERRARIUM_WAYLAND_DISPLAY_H
#define TERRARIUM_WAYLAND_DISPLAY_H

#include "src/wayland-client-core.h"
#include "src/wayland-client-protocol.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct terra_wayland_connection {
    struct wl_display *display;
    int connected;
} terra_wayland_connection_t;

void terra_wayland_connection_init(terra_wayland_connection_t *connection);
int terra_wayland_connection_open(terra_wayland_connection_t *connection, struct wl_display *display);
void terra_wayland_connection_close(terra_wayland_connection_t *connection);
int terra_wayland_connection_is_open(const terra_wayland_connection_t *connection);

#ifdef __cplusplus
}
#endif

#endif // TERRARIUM_WAYLAND_DISPLAY_H
