#ifndef TERRARIUM_NOVA_DE_H
#define TERRARIUM_NOVA_DE_H

#include "network-tray.h"
#include "statusbar.h"
#include "workspaces.h"

typedef struct nova_desktop_environment {
    nova_statusbar_t statusbar;
    nova_network_tray_t network_tray;
    nova_workspace_manager_t workspaces;
} nova_desktop_environment_t;

#endif
