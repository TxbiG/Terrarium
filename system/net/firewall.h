#ifndef TERRARIUM_SYSTEM_NET_FIREWALL_H
#define TERRARIUM_SYSTEM_NET_FIREWALL_H

#include <stdint.h>

typedef enum terra_firewall_action {
    TERRA_FIREWALL_DROP = 0,
    TERRA_FIREWALL_ACCEPT
} terra_firewall_action_t;

typedef struct terra_firewall_rule {
    uint16_t port;
    uint8_t protocol;
    terra_firewall_action_t action;
} terra_firewall_rule_t;

int terra_firewall_add_rule(const terra_firewall_rule_t *rule);
int terra_firewall_remove_rule(const terra_firewall_rule_t *rule);
int terra_firewall_set_default(terra_firewall_action_t action);

#endif
