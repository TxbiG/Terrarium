#include "firewall.h"
#include "../runtime/syscall.h"

int terra_firewall_add_rule(const terra_firewall_rule_t *rule) {
    return (int)terra_syscall(TERRA_SYSCALL_NET_FIREWALL_ADD, (uintptr_t)rule, 0, 0, 0, 0, 0);
}

int terra_firewall_remove_rule(const terra_firewall_rule_t *rule) {
    (void)rule;
    return -38;
}

int terra_firewall_set_default(terra_firewall_action_t action) {
    (void)action;
    return -38;
}
