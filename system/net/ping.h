#ifndef TERRARIUM_SYSTEM_NET_PING_H
#define TERRARIUM_SYSTEM_NET_PING_H

#include <stdint.h>
#include "ip.h"

typedef struct terra_ping_result {
    int reachable;
    uint32_t round_trip_ms;
    uint32_t ttl;
} terra_ping_result_t;

int terra_ping(const terra_ip_addr_t *addr, terra_ping_result_t *out_result);

#endif
