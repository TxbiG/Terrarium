#ifndef TERRARIUM_DRIVERS_NIC_H
#define TERRARIUM_DRIVERS_NIC_H

#include <bootloader.h>
#include <stddef.h>
#include <stdint.h>
#include "../../net/net.h"

#ifdef __cplusplus
extern "C" {
#endif

#define TERRA_NIC_MAX_DEVICES 16u
#define TERRA_NIC_NAME_MAX 32u

typedef uint32_t terra_nic_id_t;
typedef int (*terra_nic_tx_fn_t)(terra_nic_id_t id, const void *packet, size_t bytes, void *context);

typedef struct terra_nic_config {
    const char *name;
    uint8_t mac[6];
    uint32_t mtu;
    terra_nic_tx_fn_t transmit;
    void *context;
} terra_nic_config_t;

typedef struct terra_nic_info {
    terra_nic_id_t id;
    terra_netif_id_t netif;
    char name[TERRA_NIC_NAME_MAX];
    uint8_t mac[6];
    uint32_t mtu;
    uint64_t rx_packets;
    uint64_t tx_packets;
    int active;
} terra_nic_info_t;

void nic_init(const terra_boot_info_t *boot_info);
void nic_shutdown(void);
int nic_register_device(const terra_nic_config_t *config, terra_nic_id_t *out_id);
int nic_unregister_device(terra_nic_id_t id);
int nic_info(terra_nic_id_t id, terra_nic_info_t *out_info);
int nic_receive(terra_nic_id_t id, uint16_t protocol, const void *packet, size_t bytes);

#ifdef __cplusplus
}
#endif

#endif
