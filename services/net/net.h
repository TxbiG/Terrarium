#ifndef TERRARIUM_NET_SERVICE_API_H
#define TERRARIUM_NET_SERVICE_API_H

typedef enum terrarium_net_link_state {
    TERRARIUM_NET_LINK_DOWN = 0,
    TERRARIUM_NET_LINK_UP = 1,
} terrarium_net_link_state_t;

int net_service_start(void);
int net_service_status(void);
terrarium_net_link_state_t net_service_link_state(void);
void net_service_set_link_state(terrarium_net_link_state_t state);

#endif
