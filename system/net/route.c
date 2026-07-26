#include "route.h"
#include "../runtime/syscall.h"

int terra_route_add(const terra_route_t *route) {
    return (int)terra_syscall(TERRA_SYSCALL_NET_ROUTE_ADD, (uintptr_t)route, 0, 0, 0, 0, 0);
}

int terra_route_remove(const terra_route_t *route) {
    (void)route;
    return -38;
}

int terra_route_list(terra_route_t *routes, unsigned long max_routes) {
    return (int)terra_syscall(TERRA_SYSCALL_NET_ROUTE_LIST, (uintptr_t)routes, max_routes, 0, 0, 0, 0);
}
