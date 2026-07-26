#include "net.h"
#include "../security/security.h"

typedef struct terra_netif_state {
    terra_netif_info_t info;
    terra_net_tx_fn_t transmit;
    void *context;
    uint8_t active;
} terra_netif_state_t;

typedef struct terra_net_protocol_state {
    uint16_t protocol;
    terra_net_rx_handler_t handler;
    void *context;
    uint8_t active;
} terra_net_protocol_state_t;

typedef struct terra_socket_state {
    terra_net_socket_info_t info;
} terra_socket_state_t;

static terra_netif_state_t interfaces[TERRA_NET_MAX_INTERFACES];
static terra_net_protocol_state_t protocols[TERRA_NET_MAX_PROTOCOLS];
static terra_socket_state_t sockets[TERRA_NET_MAX_SOCKETS];
static terra_net_route_t routes[TERRA_NET_MAX_ROUTES];
static terra_net_addr_t dns_servers[TERRA_NET_MAX_DNS_SERVERS];
static uint8_t dns_server_active[TERRA_NET_MAX_DNS_SERVERS];
static terra_net_firewall_rule_t firewall_rules[TERRA_NET_MAX_FIREWALL_RULES];
static terra_net_packet_diag_t packet_diag[TERRA_NET_PACKET_DIAG_RING];
static terra_netif_id_t next_interface_id;
static terra_socket_id_t next_socket_id;
static uint64_t next_packet_sequence;
static int net_ready;
static terra_net_status_t link_status;
static terra_net_firewall_action_t firewall_default_action;

static void net_copy(void *dest, const void *src, size_t size) {
    uint8_t *d = (uint8_t *)dest;
    const uint8_t *s = (const uint8_t *)src;
    for (size_t i = 0; i < size; ++i)
        d[i] = s[i];
}

static int net_same(const void *a, const void *b, size_t size) {
    const uint8_t *aa = (const uint8_t *)a;
    const uint8_t *bb = (const uint8_t *)b;
    for (size_t i = 0; i < size; ++i) {
        if (aa[i] != bb[i])
            return 0;
    }
    return 1;
}

static void net_copy_name(char *dest, const char *src) {
    uint32_t i = 0;
    if (src) {
        for (; i + 1 < TERRA_NET_IF_NAME_MAX && src[i]; ++i)
            dest[i] = src[i];
    }
    dest[i] = 0;
}

static terra_netif_state_t *find_interface(terra_netif_id_t id) {
    for (uint32_t i = 0; i < TERRA_NET_MAX_INTERFACES; ++i) {
        if (interfaces[i].active && interfaces[i].info.id == id)
            return &interfaces[i];
    }
    return 0;
}

static terra_socket_state_t *find_socket(terra_socket_id_t id) {
    for (uint32_t i = 0; i < TERRA_NET_MAX_SOCKETS; ++i) {
        if (sockets[i].info.active && sockets[i].info.id == id)
            return &sockets[i];
    }
    return 0;
}

static int security_net_allowed(terra_security_action_t action,
                                uint32_t object,
                                terra_security_caps_t caps) {
    terra_security_context_t kernel = {
        .subject_id = 0,
        .subject_type = TERRA_SECURITY_SUBJECT_KERNEL,
        .caps = security_kernel_caps(),
    };
    return security_check(&kernel, action, object, caps) == TERRA_SECURITY_ALLOW;
}

static void recompute_link_status(void) {
    link_status = TERRA_NET_DOWN;
    for (uint32_t i = 0; i < TERRA_NET_MAX_INTERFACES; ++i) {
        if (interfaces[i].active && interfaces[i].info.status == TERRA_NET_UP) {
            link_status = TERRA_NET_UP;
            return;
        }
    }
}

static void record_packet_diag(terra_net_packet_direction_t direction,
                               terra_netif_id_t interface_id,
                               uint16_t protocol,
                               size_t length,
                               terra_net_result_t result) {
    uint32_t slot = (uint32_t)(next_packet_sequence % TERRA_NET_PACKET_DIAG_RING);
    packet_diag[slot].sequence = ++next_packet_sequence;
    packet_diag[slot].direction = direction;
    packet_diag[slot].interface_id = interface_id;
    packet_diag[slot].protocol = protocol;
    packet_diag[slot].length = length;
    packet_diag[slot].result = result;
}

static int firewall_matches(const terra_net_firewall_rule_t *rule,
                            terra_net_packet_direction_t direction,
                            uint16_t ether_type) {
    if (!rule || !rule->active)
        return 0;
    if (rule->direction != direction)
        return 0;
    if (rule->ether_type && rule->ether_type != ether_type)
        return 0;
    return 1;
}

static terra_net_result_t firewall_check(terra_net_packet_direction_t direction,
                                         uint16_t ether_type) {
    for (uint32_t i = 0; i < TERRA_NET_MAX_FIREWALL_RULES; ++i) {
        if (!firewall_matches(&firewall_rules[i], direction, ether_type))
            continue;
        return firewall_rules[i].action == TERRA_NET_FIREWALL_ACCEPT ? TERRA_NET_OK : TERRA_NET_ERR_DENIED;
    }
    return firewall_default_action == TERRA_NET_FIREWALL_ACCEPT ? TERRA_NET_OK : TERRA_NET_ERR_DENIED;
}

void net_init(const terra_boot_info_t *boot_info) {
    (void)boot_info;
    for (uint32_t i = 0; i < TERRA_NET_MAX_INTERFACES; ++i)
        interfaces[i].active = 0;
    for (uint32_t i = 0; i < TERRA_NET_MAX_PROTOCOLS; ++i)
        protocols[i].active = 0;
    for (uint32_t i = 0; i < TERRA_NET_MAX_SOCKETS; ++i)
        sockets[i].info.active = 0;
    for (uint32_t i = 0; i < TERRA_NET_MAX_ROUTES; ++i)
        routes[i].active = 0;
    for (uint32_t i = 0; i < TERRA_NET_MAX_DNS_SERVERS; ++i)
        dns_server_active[i] = 0;
    for (uint32_t i = 0; i < TERRA_NET_MAX_FIREWALL_RULES; ++i)
        firewall_rules[i].active = 0;
    for (uint32_t i = 0; i < TERRA_NET_PACKET_DIAG_RING; ++i)
        packet_diag[i].sequence = 0;

    next_interface_id = 1;
    next_socket_id = 1;
    next_packet_sequence = 0;
    firewall_default_action = TERRA_NET_FIREWALL_ACCEPT;
    link_status = TERRA_NET_DOWN;
    net_ready = 1;
}

int net_is_ready(void) {
    return net_ready;
}

void net_shutdown(void) {
    for (uint32_t i = 0; i < TERRA_NET_MAX_INTERFACES; ++i)
        interfaces[i].active = 0;
    for (uint32_t i = 0; i < TERRA_NET_MAX_PROTOCOLS; ++i)
        protocols[i].active = 0;
    for (uint32_t i = 0; i < TERRA_NET_MAX_SOCKETS; ++i)
        sockets[i].info.active = 0;
    link_status = TERRA_NET_DOWN;
    net_ready = 0;
}

terra_net_status_t net_status(void) {
    return link_status;
}

void net_set_link_up(int is_up) {
    link_status = is_up ? TERRA_NET_UP : TERRA_NET_DOWN;
}

terra_net_result_t netif_register(const terra_netif_config_t *config,
                                  terra_netif_id_t *out_id) {
    if (!net_ready)
        return TERRA_NET_ERR_NOT_READY;
    if (!config || !config->name || !out_id || !config->transmit)
        return TERRA_NET_ERR_INVALID;
    if (!security_net_allowed(TERRA_SECURITY_ACTION_NET_INTERFACE,
                              next_interface_id,
                              TERRA_CAP_NET_ADMIN))
        return TERRA_NET_ERR_DENIED;

    for (uint32_t i = 0; i < TERRA_NET_MAX_INTERFACES; ++i) {
        if (interfaces[i].active)
            continue;

        terra_netif_state_t *iface = &interfaces[i];
        iface->active = 1;
        iface->info.id = next_interface_id++;
        iface->info.status = TERRA_NET_DOWN;
        iface->info.mtu = config->mtu ? config->mtu : 1500u;
        iface->info.hwaddr_len = config->hwaddr_len;
        iface->info.address_len = 0;
        iface->info.prefix_len = 0;
        iface->info.rx_packets = 0;
        iface->info.tx_packets = 0;
        iface->info.rx_bytes = 0;
        iface->info.tx_bytes = 0;
        iface->info.dropped_packets = 0;
        iface->transmit = config->transmit;
        iface->context = config->context;
        net_copy_name(iface->info.name, config->name);
        if (iface->info.hwaddr_len > TERRA_NET_HWADDR_BYTES)
            iface->info.hwaddr_len = TERRA_NET_HWADDR_BYTES;
        net_copy(iface->info.hwaddr, config->hwaddr, iface->info.hwaddr_len);
        *out_id = iface->info.id;
        return TERRA_NET_OK;
    }

    return TERRA_NET_ERR_NO_SPACE;
}

terra_net_result_t netif_unregister(terra_netif_id_t id) {
    terra_netif_state_t *iface = find_interface(id);
    if (!iface)
        return TERRA_NET_ERR_NOT_FOUND;
    iface->active = 0;
    recompute_link_status();
    return TERRA_NET_OK;
}

terra_net_result_t netif_set_status(terra_netif_id_t id, terra_net_status_t status) {
    terra_netif_state_t *iface = find_interface(id);
    if (!iface)
        return TERRA_NET_ERR_NOT_FOUND;
    iface->info.status = status;
    recompute_link_status();
    return TERRA_NET_OK;
}

terra_net_result_t netif_set_address(terra_netif_id_t id,
                                     const void *address,
                                     size_t address_len) {
    terra_netif_state_t *iface = find_interface(id);
    if (!iface)
        return TERRA_NET_ERR_NOT_FOUND;
    if (!address || address_len > TERRA_NET_ADDR_BYTES)
        return TERRA_NET_ERR_INVALID;
    net_copy(iface->info.address, address, address_len);
    iface->info.address_len = (uint8_t)address_len;
    return TERRA_NET_OK;
}

terra_net_result_t netif_set_ip_config(terra_netif_id_t id,
                                       const terra_net_addr_t *address,
                                       uint8_t prefix_len) {
    if (!address)
        return TERRA_NET_ERR_INVALID;
    terra_net_result_t result = netif_set_address(id, address->bytes, address->family == 4 ? 4u : 16u);
    if (result != TERRA_NET_OK)
        return result;
    terra_netif_state_t *iface = find_interface(id);
    if (iface)
        iface->info.prefix_len = prefix_len;
    return TERRA_NET_OK;
}

terra_net_result_t netif_info(terra_netif_id_t id, terra_netif_info_t *out_info) {
    terra_netif_state_t *iface = find_interface(id);
    if (!iface)
        return TERRA_NET_ERR_NOT_FOUND;
    if (!out_info)
        return TERRA_NET_ERR_INVALID;
    *out_info = iface->info;
    return TERRA_NET_OK;
}

terra_net_result_t netif_info_by_index(uint32_t index, terra_netif_info_t *out_info) {
    uint32_t seen = 0;
    if (!out_info)
        return TERRA_NET_ERR_INVALID;
    for (uint32_t i = 0; i < TERRA_NET_MAX_INTERFACES; ++i) {
        if (!interfaces[i].active)
            continue;
        if (seen == index) {
            *out_info = interfaces[i].info;
            return TERRA_NET_OK;
        }
        seen++;
    }
    return TERRA_NET_ERR_NOT_FOUND;
}

int netif_count(void) {
    int count = 0;
    for (uint32_t i = 0; i < TERRA_NET_MAX_INTERFACES; ++i) {
        if (interfaces[i].active)
            count++;
    }
    return count;
}

terra_net_result_t net_protocol_register(uint16_t protocol,
                                         terra_net_rx_handler_t handler,
                                         void *context) {
    if (!handler)
        return TERRA_NET_ERR_INVALID;
    for (uint32_t i = 0; i < TERRA_NET_MAX_PROTOCOLS; ++i) {
        if (protocols[i].active && protocols[i].protocol == protocol &&
            protocols[i].handler == handler && protocols[i].context == context)
            return TERRA_NET_ERR_INVALID;
    }
    for (uint32_t i = 0; i < TERRA_NET_MAX_PROTOCOLS; ++i) {
        if (protocols[i].active)
            continue;
        protocols[i].active = 1;
        protocols[i].protocol = protocol;
        protocols[i].handler = handler;
        protocols[i].context = context;
        return TERRA_NET_OK;
    }
    return TERRA_NET_ERR_NO_SPACE;
}

terra_net_result_t net_protocol_unregister(uint16_t protocol,
                                           terra_net_rx_handler_t handler,
                                           void *context) {
    for (uint32_t i = 0; i < TERRA_NET_MAX_PROTOCOLS; ++i) {
        if (protocols[i].active && protocols[i].protocol == protocol &&
            protocols[i].handler == handler && protocols[i].context == context) {
            protocols[i].active = 0;
            return TERRA_NET_OK;
        }
    }
    return TERRA_NET_ERR_NOT_FOUND;
}

terra_net_result_t net_receive_packet(terra_netif_id_t id,
                                      uint16_t protocol,
                                      const void *packet,
                                      size_t length) {
    terra_netif_state_t *iface = find_interface(id);
    terra_net_result_t result;

    if (!iface)
        return TERRA_NET_ERR_NOT_FOUND;
    if (!packet || !length)
        return TERRA_NET_ERR_INVALID;

    result = firewall_check(TERRA_NET_PACKET_RX, protocol);
    if (result != TERRA_NET_OK) {
        iface->info.dropped_packets++;
        record_packet_diag(TERRA_NET_PACKET_RX, id, protocol, length, result);
        return result;
    }

    iface->info.rx_packets++;
    iface->info.rx_bytes += length;

    result = TERRA_NET_ERR_NOT_FOUND;
    for (uint32_t i = 0; i < TERRA_NET_MAX_PROTOCOLS; ++i) {
        if (!protocols[i].active)
            continue;
        if (protocols[i].protocol != protocol && protocols[i].protocol != TERRA_NET_PROTO_RAW)
            continue;
        result = protocols[i].handler(id, packet, length, protocols[i].context);
    }
    if (result != TERRA_NET_OK)
        iface->info.dropped_packets++;
    record_packet_diag(TERRA_NET_PACKET_RX, id, protocol, length, result);
    return result;
}

terra_net_result_t net_transmit_packet(terra_netif_id_t id,
                                       const void *packet,
                                       size_t length) {
    terra_netif_state_t *iface = find_interface(id);
    terra_net_result_t result;

    if (!iface)
        return TERRA_NET_ERR_NOT_FOUND;
    if (!packet || !length)
        return TERRA_NET_ERR_INVALID;
    if (iface->info.status != TERRA_NET_UP)
        return TERRA_NET_ERR_DOWN;

    result = firewall_check(TERRA_NET_PACKET_TX, TERRA_NET_PROTO_RAW);
    if (result != TERRA_NET_OK) {
        iface->info.dropped_packets++;
        record_packet_diag(TERRA_NET_PACKET_TX, id, TERRA_NET_PROTO_RAW, length, result);
        return result;
    }

    result = iface->transmit(id, packet, length, iface->context);
    if (result == TERRA_NET_OK) {
        iface->info.tx_packets++;
        iface->info.tx_bytes += length;
    } else {
        iface->info.dropped_packets++;
    }
    record_packet_diag(TERRA_NET_PACKET_TX, id, TERRA_NET_PROTO_RAW, length, result);
    return result;
}

terra_net_result_t net_socket_open(uint16_t protocol,
                                   terra_netif_id_t bind_interface,
                                   terra_socket_id_t *out_socket) {
    if (!out_socket)
        return TERRA_NET_ERR_INVALID;
    if (!security_net_allowed(protocol == TERRA_NET_PROTO_RAW
                                  ? TERRA_SECURITY_ACTION_NET_RAW
                                  : TERRA_SECURITY_ACTION_NET_SOCKET,
                              protocol,
                              protocol == TERRA_NET_PROTO_RAW ? TERRA_CAP_NET_RAW : TERRA_CAP_NET_BIND))
        return TERRA_NET_ERR_DENIED;
    if (bind_interface && !find_interface(bind_interface))
        return TERRA_NET_ERR_NOT_FOUND;

    for (uint32_t i = 0; i < TERRA_NET_MAX_SOCKETS; ++i) {
        if (sockets[i].info.active)
            continue;
        sockets[i].info.active = 1;
        sockets[i].info.id = next_socket_id++;
        sockets[i].info.protocol = protocol;
        sockets[i].info.bind_interface = bind_interface;
        sockets[i].info.rx_packets = 0;
        sockets[i].info.tx_packets = 0;
        sockets[i].info.rx_bytes = 0;
        sockets[i].info.tx_bytes = 0;
        *out_socket = sockets[i].info.id;
        return TERRA_NET_OK;
    }
    return TERRA_NET_ERR_NO_SPACE;
}

terra_net_result_t net_socket_close(terra_socket_id_t socket) {
    terra_socket_state_t *state = find_socket(socket);
    if (!state)
        return TERRA_NET_ERR_NOT_FOUND;
    state->info.active = 0;
    return TERRA_NET_OK;
}

terra_net_result_t net_socket_info(terra_socket_id_t socket, terra_net_socket_info_t *out_info) {
    terra_socket_state_t *state = find_socket(socket);
    if (!state)
        return TERRA_NET_ERR_NOT_FOUND;
    if (!out_info)
        return TERRA_NET_ERR_INVALID;
    *out_info = state->info;
    return TERRA_NET_OK;
}

terra_net_result_t net_socket_info_by_index(uint32_t index, terra_net_socket_info_t *out_info) {
    uint32_t seen = 0;
    if (!out_info)
        return TERRA_NET_ERR_INVALID;
    for (uint32_t i = 0; i < TERRA_NET_MAX_SOCKETS; ++i) {
        if (!sockets[i].info.active)
            continue;
        if (seen == index) {
            *out_info = sockets[i].info;
            return TERRA_NET_OK;
        }
        seen++;
    }
    return TERRA_NET_ERR_NOT_FOUND;
}

int net_socket_count(void) {
    int count = 0;
    for (uint32_t i = 0; i < TERRA_NET_MAX_SOCKETS; ++i) {
        if (sockets[i].info.active)
            count++;
    }
    return count;
}

terra_net_result_t net_route_add(const terra_net_route_t *route) {
    if (!route || !find_interface(route->interface_id))
        return TERRA_NET_ERR_INVALID;
    for (uint32_t i = 0; i < TERRA_NET_MAX_ROUTES; ++i) {
        if (routes[i].active)
            continue;
        routes[i] = *route;
        routes[i].active = 1;
        return TERRA_NET_OK;
    }
    return TERRA_NET_ERR_NO_SPACE;
}

terra_net_result_t net_route_remove(const terra_net_route_t *route) {
    if (!route)
        return TERRA_NET_ERR_INVALID;
    for (uint32_t i = 0; i < TERRA_NET_MAX_ROUTES; ++i) {
        if (!routes[i].active)
            continue;
        if (routes[i].interface_id == route->interface_id &&
            routes[i].prefix_len == route->prefix_len &&
            routes[i].destination.family == route->destination.family &&
            net_same(routes[i].destination.bytes, route->destination.bytes, TERRA_NET_ADDR_BYTES)) {
            routes[i].active = 0;
            return TERRA_NET_OK;
        }
    }
    return TERRA_NET_ERR_NOT_FOUND;
}

terra_net_result_t net_route_get(uint32_t index, terra_net_route_t *out_route) {
    uint32_t seen = 0;
    if (!out_route)
        return TERRA_NET_ERR_INVALID;
    for (uint32_t i = 0; i < TERRA_NET_MAX_ROUTES; ++i) {
        if (!routes[i].active)
            continue;
        if (seen == index) {
            *out_route = routes[i];
            return TERRA_NET_OK;
        }
        seen++;
    }
    return TERRA_NET_ERR_NOT_FOUND;
}

int net_route_count(void) {
    int count = 0;
    for (uint32_t i = 0; i < TERRA_NET_MAX_ROUTES; ++i) {
        if (routes[i].active)
            count++;
    }
    return count;
}

terra_net_result_t net_dns_set_server(uint32_t index, const terra_net_addr_t *server) {
    if (index >= TERRA_NET_MAX_DNS_SERVERS || !server)
        return TERRA_NET_ERR_INVALID;
    dns_servers[index] = *server;
    dns_server_active[index] = 1;
    return TERRA_NET_OK;
}

terra_net_result_t net_dns_get_server(uint32_t index, terra_net_addr_t *out_server) {
    if (index >= TERRA_NET_MAX_DNS_SERVERS || !out_server)
        return TERRA_NET_ERR_INVALID;
    if (!dns_server_active[index])
        return TERRA_NET_ERR_NOT_FOUND;
    *out_server = dns_servers[index];
    return TERRA_NET_OK;
}

int net_dns_server_count(void) {
    int count = 0;
    for (uint32_t i = 0; i < TERRA_NET_MAX_DNS_SERVERS; ++i) {
        if (dns_server_active[i])
            count++;
    }
    return count;
}

terra_net_result_t net_firewall_add_rule(const terra_net_firewall_rule_t *rule) {
    if (!rule)
        return TERRA_NET_ERR_INVALID;
    for (uint32_t i = 0; i < TERRA_NET_MAX_FIREWALL_RULES; ++i) {
        if (firewall_rules[i].active)
            continue;
        firewall_rules[i] = *rule;
        firewall_rules[i].active = 1;
        return TERRA_NET_OK;
    }
    return TERRA_NET_ERR_NO_SPACE;
}

terra_net_result_t net_firewall_remove_rule(uint32_t index) {
    uint32_t seen = 0;
    for (uint32_t i = 0; i < TERRA_NET_MAX_FIREWALL_RULES; ++i) {
        if (!firewall_rules[i].active)
            continue;
        if (seen == index) {
            firewall_rules[i].active = 0;
            return TERRA_NET_OK;
        }
        seen++;
    }
    return TERRA_NET_ERR_NOT_FOUND;
}

terra_net_result_t net_firewall_get_rule(uint32_t index, terra_net_firewall_rule_t *out_rule) {
    uint32_t seen = 0;
    if (!out_rule)
        return TERRA_NET_ERR_INVALID;
    for (uint32_t i = 0; i < TERRA_NET_MAX_FIREWALL_RULES; ++i) {
        if (!firewall_rules[i].active)
            continue;
        if (seen == index) {
            *out_rule = firewall_rules[i];
            return TERRA_NET_OK;
        }
        seen++;
    }
    return TERRA_NET_ERR_NOT_FOUND;
}

terra_net_result_t net_firewall_set_default(terra_net_firewall_action_t action) {
    firewall_default_action = action;
    return TERRA_NET_OK;
}

terra_net_firewall_action_t net_firewall_default(void) {
    return firewall_default_action;
}

int net_firewall_rule_count(void) {
    int count = 0;
    for (uint32_t i = 0; i < TERRA_NET_MAX_FIREWALL_RULES; ++i) {
        if (firewall_rules[i].active)
            count++;
    }
    return count;
}

terra_net_result_t net_packet_diag_get(uint32_t newest_index, terra_net_packet_diag_t *out_diag) {
    uint64_t available = next_packet_sequence < TERRA_NET_PACKET_DIAG_RING ? next_packet_sequence : TERRA_NET_PACKET_DIAG_RING;
    uint64_t sequence;
    uint32_t slot;

    if (!out_diag)
        return TERRA_NET_ERR_INVALID;
    if (newest_index >= available)
        return TERRA_NET_ERR_NOT_FOUND;

    sequence = next_packet_sequence - newest_index;
    slot = (uint32_t)((sequence - 1u) % TERRA_NET_PACKET_DIAG_RING);
    *out_diag = packet_diag[slot];
    return TERRA_NET_OK;
}

int net_packet_diag_count(void) {
    return (int)(next_packet_sequence < TERRA_NET_PACKET_DIAG_RING ? next_packet_sequence : TERRA_NET_PACKET_DIAG_RING);
}
