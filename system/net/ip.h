#ifndef TERRARIUM_SYSTEM_NET_IP_H
#define TERRARIUM_SYSTEM_NET_IP_H

#include <stdint.h>

typedef struct terra_ip_addr {
    uint8_t family;
    union {
        uint32_t ipv4;
        uint8_t ipv6[16];
    } addr;
} terra_ip_addr_t;

int terra_ip_parse(const char *text, terra_ip_addr_t *out_addr);
int terra_ip_format(const terra_ip_addr_t *addr, char *out_text, unsigned long out_size);
int terra_ip_assign(const char *interface_name, const terra_ip_addr_t *addr, uint8_t prefix_len);

#endif
