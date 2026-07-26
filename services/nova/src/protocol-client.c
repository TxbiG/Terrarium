#include "protocol-client.h"

#include <string.h>

static void nova_copy_text(char *dst, unsigned cap, const char *src) {
    unsigned i = 0;
    if (!dst || cap == 0)
        return;
    if (!src)
        src = "";
    while (src[i] && i + 1 < cap) {
        dst[i] = src[i];
        ++i;
    }
    dst[i] = 0;
}

void nova_protocol_client_init(nova_protocol_client_t *client) {
    if (!client)
        return;

    memset(client, 0, sizeof(*client));
    client->protocol_version = TERRA_DESKTOP_PROTOCOL_VERSION;
    client->taskbar_layer.role = TERRA_SHELL_SURFACE_PANEL;
    client->taskbar_layer.anchor = TERRA_SHELL_EDGE_BOTTOM | TERRA_SHELL_EDGE_LEFT | TERRA_SHELL_EDGE_RIGHT;
    client->taskbar_layer.keyboard_interactive = 0;
    client->launcher_layer.role = TERRA_SHELL_SURFACE_LAUNCHER;
    client->launcher_layer.anchor = TERRA_SHELL_EDGE_BOTTOM | TERRA_SHELL_EDGE_LEFT;
    client->launcher_layer.keyboard_interactive = 1;
}

int nova_protocol_client_connect(nova_protocol_client_t *client, const char *compositor_name) {
    if (!client)
        return -1;

    client->connected = 1;
    nova_copy_text(client->compositor_name, sizeof(client->compositor_name), compositor_name ? compositor_name : "lumina");
    return 0;
}

void nova_protocol_client_disconnect(nova_protocol_client_t *client) {
    if (!client)
        return;

    client->connected = 0;
}

int nova_protocol_client_is_connected(const nova_protocol_client_t *client) {
    return client ? client->connected : 0;
}
