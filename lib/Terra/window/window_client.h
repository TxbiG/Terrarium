#ifndef TERRA_WINDOW_CLIENT_H
#define TERRA_WINDOW_CLIENT_H

#include "display_protocol.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum terra_window_client_state {
    TERRA_WINDOW_CLIENT_DISCONNECTED = 0,
    TERRA_WINDOW_CLIENT_CONNECTED,
    TERRA_WINDOW_CLIENT_CLOSED
} terra_window_client_state_t;

typedef struct terra_window_client {
    terra_window_client_state_t state;
    terra_display_view_info_t view;
    uint32_t configured;
    uint32_t close_requested;
    uint32_t event_count;
} terra_window_client_t;

void terra_window_client_init(terra_window_client_t *client, const char *title, int width, int height);
int terra_window_client_connect(terra_window_client_t *client, const char *display_name);
void terra_window_client_handle_event(terra_window_client_t *client, const terra_display_event_t *event);
void terra_window_client_configure(terra_window_client_t *client, int width, int height);
void terra_window_client_request_close(terra_window_client_t *client);
int terra_window_client_should_close(const terra_window_client_t *client);
void terra_window_client_disconnect(terra_window_client_t *client);

#ifdef __cplusplus
}
#endif

#endif