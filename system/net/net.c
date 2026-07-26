#include "net.h"
#include "../runtime/syscall.h"

int terra_net_list_interfaces(terra_net_interface_t *interfaces, unsigned long max_interfaces) {
    return (int)terra_syscall(TERRA_SYSCALL_NET_IF_LIST, (uintptr_t)interfaces, max_interfaces, 0, 0, 0, 0);
}

int terra_net_set_state(const char *interface_name, terra_net_state_t state) {
    (void)interface_name;
    (void)state;
    return -38;
}
