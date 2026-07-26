#include "Display.h"

void terra_wayland_connection_init(terra_wayland_connection_t *connection) {
    if (!connection)
        return;

    connection->display = 0;
    connection->connected = 0;
}

int terra_wayland_connection_open(terra_wayland_connection_t *connection, struct wl_display *display) {
    if (!connection || !display)
        return -1;

    connection->display = display;
    connection->connected = 1;
    return 0;
}

void terra_wayland_connection_close(terra_wayland_connection_t *connection) {
    if (!connection)
        return;

    connection->display = 0;
    connection->connected = 0;
}

int terra_wayland_connection_is_open(const terra_wayland_connection_t *connection) {
    return connection ? connection->connected : 0;
}
