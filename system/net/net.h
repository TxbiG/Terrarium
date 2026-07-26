#ifndef TERRARIUM_SYSTEM_NET_NET_H
#define TERRARIUM_SYSTEM_NET_NET_H

#include <stdint.h>

typedef enum terra_net_state {
    TERRA_NET_STATE_DOWN = 0,
    TERRA_NET_STATE_UP,
    TERRA_NET_STATE_CONFIGURING
} terra_net_state_t;

typedef struct terra_net_interface {
    char name[32];
    uint8_t mac[6];
    terra_net_state_t state;
    uint64_t rx_bytes;
    uint64_t tx_bytes;
} terra_net_interface_t;

int terra_net_list_interfaces(terra_net_interface_t *interfaces, unsigned long max_interfaces);
int terra_net_set_state(const char *interface_name, terra_net_state_t state);

#endif
