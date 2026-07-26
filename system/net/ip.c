#include "ip.h"

#include <stdio.h>
#include <string.h>
#include "../runtime/syscall.h"

static int parse_u8(const char **text, unsigned *out) {
    unsigned value = 0;
    unsigned digits = 0;
    while (**text >= '0' && **text <= '9') {
        value = value * 10u + (unsigned)(**text - '0');
        if (value > 255u)
            return 0;
        (*text)++;
        digits++;
    }
    if (digits == 0)
        return 0;
    *out = value;
    return 1;
}

int terra_ip_parse(const char *text, terra_ip_addr_t *out_addr) {
    unsigned parts[4];
    const char *p = text;

    if (!text || !out_addr)
        return -22;

    for (unsigned i = 0; i < 4; ++i) {
        if (!parse_u8(&p, &parts[i]))
            return -22;
        if (i < 3) {
            if (*p != '.')
                return -22;
            ++p;
        }
    }
    if (*p != 0)
        return -22;

    memset(out_addr, 0, sizeof(*out_addr));
    out_addr->family = 4;
    out_addr->addr.ipv4 = (parts[0] << 24u) | (parts[1] << 16u) | (parts[2] << 8u) | parts[3];
    return 0;
}

int terra_ip_format(const terra_ip_addr_t *addr, char *out_text, unsigned long out_size) {
    unsigned a;
    if (!addr || !out_text || out_size == 0)
        return -22;
    if (addr->family != 4)
        return -95;
    a = addr->addr.ipv4;
    snprintf(out_text, out_size, "%u.%u.%u.%u", (a >> 24u) & 255u, (a >> 16u) & 255u, (a >> 8u) & 255u, a & 255u);
    return 0;
}

int terra_ip_assign(const char *interface_name, const terra_ip_addr_t *addr, uint8_t prefix_len) {
    (void)interface_name;
    (void)addr;
    (void)prefix_len;
    return -38;
}
