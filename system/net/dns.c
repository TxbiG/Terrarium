#include "dns.h"
#include "../runtime/syscall.h"

int terra_dns_resolve(const char *hostname, terra_ip_addr_t *out_addr) {
    (void)hostname;
    (void)out_addr;
    return -38;
}

int terra_dns_set_server(const terra_ip_addr_t *server) {
    return (int)terra_syscall(TERRA_SYSCALL_NET_DNS_SET, 0, (uintptr_t)server, 0, 0, 0, 0);
}

int terra_dns_flush_cache(void) {
    return -38;
}
