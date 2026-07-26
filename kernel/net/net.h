#ifndef TERRARIUM_NET_H
#define TERRARIUM_NET_H

#include <bootloader.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum terra_net_status {
    TERRA_NET_DOWN = 0,
    TERRA_NET_UP = 1
} terra_net_status_t;

typedef enum terra_net_result {
    TERRA_NET_OK = 0,
    TERRA_NET_ERR_NOT_READY = -1,
    TERRA_NET_ERR_INVALID = -2,
    TERRA_NET_ERR_NO_SPACE = -3,
    TERRA_NET_ERR_NOT_FOUND = -4,
    TERRA_NET_ERR_DENIED = -5,
    TERRA_NET_ERR_DOWN = -6
} terra_net_result_t;

typedef enum terra_net_protocol {
    TERRA_NET_PROTO_RAW = 0,
    TERRA_NET_PROTO_IPV4 = 0x0800,
    TERRA_NET_PROTO_ARP = 0x0806,
    TERRA_NET_PROTO_IPV6 = 0x86dd
} terra_net_protocol_t;

typedef enum terra_net_transport {
    TERRA_NET_TRANSPORT_ANY = 0,
    TERRA_NET_TRANSPORT_ICMP = 1,
    TERRA_NET_TRANSPORT_TCP = 6,
    TERRA_NET_TRANSPORT_UDP = 17
} terra_net_transport_t;

typedef enum terra_net_firewall_action {
    TERRA_NET_FIREWALL_DROP = 0,
    TERRA_NET_FIREWALL_ACCEPT = 1
} terra_net_firewall_action_t;

typedef enum terra_net_packet_direction {
    TERRA_NET_PACKET_RX = 0,
    TERRA_NET_PACKET_TX = 1
} terra_net_packet_direction_t;

#define TERRA_NET_MAX_INTERFACES 16u
#define TERRA_NET_MAX_PROTOCOLS 32u
#define TERRA_NET_MAX_SOCKETS 128u
#define TERRA_NET_MAX_ROUTES 64u
#define TERRA_NET_MAX_DNS_SERVERS 4u
#define TERRA_NET_MAX_FIREWALL_RULES 64u
#define TERRA_NET_PACKET_DIAG_RING 128u
#define TERRA_NET_ADDR_BYTES 16u
#define TERRA_NET_HWADDR_BYTES 16u
#define TERRA_NET_IF_NAME_MAX 16u
#define TERRA_NET_HOSTNAME_MAX 64u

typedef uint32_t terra_netif_id_t;
typedef uint32_t terra_socket_id_t;

typedef terra_net_result_t (*terra_net_rx_handler_t)(terra_netif_id_t netif,
                                                     const void *packet,
                                                     size_t length,
                                                     void *context);
typedef terra_net_result_t (*terra_net_tx_fn_t)(terra_netif_id_t netif,
                                                const void *packet,
                                                size_t length,
                                                void *context);

typedef struct terra_net_addr {
    uint8_t family;
    uint8_t bytes[TERRA_NET_ADDR_BYTES];
} terra_net_addr_t;

typedef struct terra_netif_config {
    const char *name;
    uint32_t mtu;
    uint8_t hwaddr[TERRA_NET_HWADDR_BYTES];
    uint8_t hwaddr_len;
    terra_net_tx_fn_t transmit;
    void *context;
} terra_netif_config_t;

typedef struct terra_netif_info {
    terra_netif_id_t id;
    char name[TERRA_NET_IF_NAME_MAX];
    terra_net_status_t status;
    uint32_t mtu;
    uint8_t hwaddr[TERRA_NET_HWADDR_BYTES];
    uint8_t hwaddr_len;
    uint8_t address[TERRA_NET_ADDR_BYTES];
    uint8_t address_len;
    uint8_t prefix_len;
    uint64_t rx_packets;
    uint64_t tx_packets;
    uint64_t rx_bytes;
    uint64_t tx_bytes;
    uint64_t dropped_packets;
} terra_netif_info_t;

typedef struct terra_net_route {
    terra_net_addr_t destination;
    uint8_t prefix_len;
    terra_net_addr_t gateway;
    terra_netif_id_t interface_id;
    uint32_t metric;
    uint8_t active;
} terra_net_route_t;

typedef struct terra_net_firewall_rule {
    terra_net_packet_direction_t direction;
    uint16_t ether_type;
    uint8_t transport;
    uint16_t local_port;
    uint16_t remote_port;
    terra_net_firewall_action_t action;
    uint8_t active;
} terra_net_firewall_rule_t;

typedef struct terra_net_socket_info {
    terra_socket_id_t id;
    uint16_t protocol;
    terra_netif_id_t bind_interface;
    uint64_t rx_packets;
    uint64_t tx_packets;
    uint64_t rx_bytes;
    uint64_t tx_bytes;
    uint8_t active;
} terra_net_socket_info_t;

typedef struct terra_net_packet_diag {
    uint64_t sequence;
    terra_net_packet_direction_t direction;
    terra_netif_id_t interface_id;
    uint16_t protocol;
    size_t length;
    terra_net_result_t result;
} terra_net_packet_diag_t;

void net_init(const terra_boot_info_t *boot_info);
int net_is_ready(void);
void net_shutdown(void);
terra_net_status_t net_status(void);
void net_set_link_up(int is_up);
terra_net_result_t netif_register(const terra_netif_config_t *config,
                                  terra_netif_id_t *out_id);
terra_net_result_t netif_unregister(terra_netif_id_t id);
terra_net_result_t netif_set_status(terra_netif_id_t id, terra_net_status_t status);
terra_net_result_t netif_set_address(terra_netif_id_t id,
                                     const void *address,
                                     size_t address_len);
terra_net_result_t netif_set_ip_config(terra_netif_id_t id,
                                       const terra_net_addr_t *address,
                                       uint8_t prefix_len);
terra_net_result_t netif_info(terra_netif_id_t id, terra_netif_info_t *out_info);
terra_net_result_t netif_info_by_index(uint32_t index, terra_netif_info_t *out_info);
int netif_count(void);
terra_net_result_t net_protocol_register(uint16_t protocol,
                                         terra_net_rx_handler_t handler,
                                         void *context);
terra_net_result_t net_protocol_unregister(uint16_t protocol,
                                           terra_net_rx_handler_t handler,
                                           void *context);
terra_net_result_t net_receive_packet(terra_netif_id_t id,
                                      uint16_t protocol,
                                      const void *packet,
                                      size_t length);
terra_net_result_t net_transmit_packet(terra_netif_id_t id,
                                       const void *packet,
                                       size_t length);
terra_net_result_t net_socket_open(uint16_t protocol,
                                   terra_netif_id_t bind_interface,
                                   terra_socket_id_t *out_socket);
terra_net_result_t net_socket_close(terra_socket_id_t socket);
terra_net_result_t net_socket_info(terra_socket_id_t socket, terra_net_socket_info_t *out_info);
terra_net_result_t net_socket_info_by_index(uint32_t index, terra_net_socket_info_t *out_info);
int net_socket_count(void);
terra_net_result_t net_route_add(const terra_net_route_t *route);
terra_net_result_t net_route_remove(const terra_net_route_t *route);
terra_net_result_t net_route_get(uint32_t index, terra_net_route_t *out_route);
int net_route_count(void);
terra_net_result_t net_dns_set_server(uint32_t index, const terra_net_addr_t *server);
terra_net_result_t net_dns_get_server(uint32_t index, terra_net_addr_t *out_server);
int net_dns_server_count(void);
terra_net_result_t net_firewall_add_rule(const terra_net_firewall_rule_t *rule);
terra_net_result_t net_firewall_remove_rule(uint32_t index);
terra_net_result_t net_firewall_get_rule(uint32_t index, terra_net_firewall_rule_t *out_rule);
terra_net_result_t net_firewall_set_default(terra_net_firewall_action_t action);
terra_net_firewall_action_t net_firewall_default(void);
int net_firewall_rule_count(void);
terra_net_result_t net_packet_diag_get(uint32_t newest_index, terra_net_packet_diag_t *out_diag);
int net_packet_diag_count(void);

#ifdef __cplusplus
}
#endif

#endif
