#ifndef TERRARIUM_NOVA_SERVER_H
#define TERRARIUM_NOVA_SERVER_H

#include "config.h"
#include "etc/network-tray.h"
#include "etc/statusbar.h"
#include "etc/workspaces.h"
#include "protocol-client.h"
#include <Terra/gfx/UI/Desktop.h>
#include <Terra/gfx/UI/Taskbar.h>
#include <Terra/gfx/UI/Widget/Widget.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct nova_server {
    terra_desktop_t desktop;
    terra_taskbar_t taskbar;
    terra_widget_t launcher_button;
    nova_statusbar_t statusbar;
    nova_network_tray_t network_tray;
    nova_workspace_manager_t workspaces;
    nova_protocol_client_t protocol;
    int running;
} nova_server_t;

int nova_server_init(nova_server_t *server, int width, int height);
void nova_server_run_once(nova_server_t *server);
void nova_server_print_status(const nova_server_t *server);
void nova_server_shutdown(nova_server_t *server);

#ifdef __cplusplus
}
#endif

#endif

