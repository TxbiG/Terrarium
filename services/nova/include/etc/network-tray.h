#ifndef TERRARIUM_NOVA_NETWORK_TRAY_H
#define TERRARIUM_NOVA_NETWORK_TRAY_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum nova_network_state {
    NOVA_NETWORK_OFFLINE = 0,
    NOVA_NETWORK_CONNECTING,
    NOVA_NETWORK_ONLINE,
} nova_network_state_t;

typedef struct nova_network_tray {
    nova_network_state_t state;
    char interface_name[16];
    int visible;
} nova_network_tray_t;

void nova_network_tray_init(nova_network_tray_t *tray);
void nova_network_tray_set_state(nova_network_tray_t *tray, nova_network_state_t state);
void nova_network_tray_update(nova_network_tray_t *tray);
const char *nova_network_state_name(nova_network_state_t state);

#ifdef __cplusplus
}
#endif

#endif
