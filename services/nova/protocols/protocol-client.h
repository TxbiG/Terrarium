#ifndef TERRARIUM_NOVA_PROTOCOL_CLIENT_H
#define TERRARIUM_NOVA_PROTOCOL_CLIENT_H

#include <Terra/window/desktop_protocol.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct nova_protocol_client {
    int connected;
    unsigned protocol_version;
    char compositor_name[TERRA_DISPLAY_NAME_MAX];
    terra_layer_surface_request_t taskbar_layer;
    terra_layer_surface_request_t launcher_layer;
} nova_protocol_client_t;

void nova_protocol_client_init(nova_protocol_client_t *client);
int nova_protocol_client_connect(nova_protocol_client_t *client, const char *compositor_name);
void nova_protocol_client_disconnect(nova_protocol_client_t *client);
int nova_protocol_client_is_connected(const nova_protocol_client_t *client);

#ifdef __cplusplus
}
#endif

#endif
