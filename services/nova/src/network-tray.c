#include "etc/network-tray.h"
#include <string.h>

void nova_network_tray_init(nova_network_tray_t *tray) {
    if (!tray) {
        return;
    }

    memset(tray, 0, sizeof(*tray));
    tray->state = NOVA_NETWORK_CONNECTING;
    strncpy(tray->interface_name, "net0", sizeof(tray->interface_name) - 1);
    tray->visible = 1;
}

void nova_network_tray_set_state(nova_network_tray_t *tray, nova_network_state_t state) {
    if (tray) {
        tray->state = state;
    }
}

void nova_network_tray_update(nova_network_tray_t *tray) {
    if (!tray) {
        return;
    }

    if (tray->state == NOVA_NETWORK_CONNECTING) {
        tray->state = NOVA_NETWORK_ONLINE;
    }
}

const char *nova_network_state_name(nova_network_state_t state) {
    switch (state) {
    case NOVA_NETWORK_OFFLINE:
        return "offline";
    case NOVA_NETWORK_CONNECTING:
        return "connecting";
    case NOVA_NETWORK_ONLINE:
        return "online";
    default:
        return "unknown";
    }
}
