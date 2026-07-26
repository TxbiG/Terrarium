#include "ping.h"
#include "../runtime/syscall.h"

int terra_ping(const terra_ip_addr_t *addr, terra_ping_result_t *out_result) {
    return (int)terra_syscall(TERRA_SYSCALL_NET_PING, (uintptr_t)addr, (uintptr_t)out_result, 0, 0, 0, 0);
}
