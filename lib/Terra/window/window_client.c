#include "window_client.h"

#include <string.h>

static void copy_text(char *dst, unsigned cap, const char *src) {
    unsigned i = 0;
    if (!dst || cap == 0)
        return;
    if (!src)
        src = "";
    while (src[i] && i + 1 < cap) {
        dst[i] = src[i];
        ++i;
    }
    dst[i] = '\0';
}

void terra_window_client_init(terra_window_client_t *client, const char *title, int width, int height) {
    if (!client)
        return;

    memset(client, 0, sizeof(*client));
    client->state = TERRA_WINDOW_CLIENT_DISCONNECTED;
    client->view.id = 1;
    copy_text(client->view.title, sizeof(client->view.title), title ? title : "Terrarium Window");
    client->view.rect.width = width;
    client->view.rect.height = height;
    client->view.mapped = 0;
    client->view.focused = 0;
}

int terra_window_client_connect(terra_window_client_t *client, const char *display_name) {
    (void)display_name;
    if (!client)
        return -1;

    client->state = TERRA_WINDOW_CLIENT_CONNECTED;
    client->view.mapped = 1;
    return 0;
}

void terra_window_client_configure(terra_window_client_t *client, int width, int height) {
    if (!client)
        return;

    if (width > 0)
        client->view.rect.width = width;
    if (height > 0)
        client->view.rect.height = height;
    client->configured = 1;
}

void terra_window_client_request_close(terra_window_client_t *client) {
    if (!client)
        return;

    client->close_requested = 1;
    client->state = TERRA_WINDOW_CLIENT_CLOSED;
    client->view.mapped = 0;
}

void terra_window_client_handle_event(terra_window_client_t *client, const terra_display_event_t *event) {
    if (!client || !event)
        return;

    client->event_count++;
    switch (event->type) {
    case TERRA_DISPLAY_EVENT_VIEW_CONFIGURE:
        if (event->target_id == client->view.id)
            terra_window_client_configure(client, (int)event->value0, (int)event->value1);
        break;
    case TERRA_DISPLAY_EVENT_VIEW_FOCUSED:
        client->view.focused = event->target_id == client->view.id;
        break;
    case TERRA_DISPLAY_EVENT_VIEW_CLOSE_REQUESTED:
        if (event->target_id == client->view.id)
            terra_window_client_request_close(client);
        break;
    case TERRA_DISPLAY_EVENT_VIEW_DESTROYED:
        if (event->target_id == client->view.id)
            terra_window_client_request_close(client);
        break;
    case TERRA_DISPLAY_EVENT_FRAME:
        break;
    default:
        break;
    }
}

int terra_window_client_should_close(const terra_window_client_t *client) {
    return !client || client->close_requested || client->state == TERRA_WINDOW_CLIENT_CLOSED;
}

void terra_window_client_disconnect(terra_window_client_t *client) {
    if (!client)
        return;

    client->state = TERRA_WINDOW_CLIENT_DISCONNECTED;
    client->view.mapped = 0;
}
