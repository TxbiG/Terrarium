#ifndef TERRARIUM_SYSTEM_NET_DNS_H
#define TERRARIUM_SYSTEM_NET_DNS_H

#include "ip.h"

int terra_dns_resolve(const char *hostname, terra_ip_addr_t *out_addr);
int terra_dns_set_server(const terra_ip_addr_t *server);
int terra_dns_flush_cache(void);

#endif
