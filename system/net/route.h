#ifndef TERRARIUM_SYSTEM_NET_ROUTE_H
#define TERRARIUM_SYSTEM_NET_ROUTE_H

#include "ip.h"

typedef struct terra_route {
    terra_ip_addr_t destination;
    uint8_t prefix_len;
    terra_ip_addr_t gateway;
    char interface_name[32];
} terra_route_t;

int terra_route_add(const terra_route_t *route);
int terra_route_remove(const terra_route_t *route);
int terra_route_list(terra_route_t *routes, unsigned long max_routes);

#endif
