#include "nic.h"

#include "../drivers.h"

typedef struct terra_nic_record {
    terra_nic_info_t info;
    terra_nic_tx_fn_t transmit;
    void *context;
} terra_nic_record_t;

static terra_nic_record_t nics[TERRA_NIC_MAX_DEVICES];
static terra_nic_id_t next_nic_id;
static int nic_ready;

static void nic_copy_name(char *dest, const char *src) {
    uint32_t i = 0;
    if (src) {
        for (; i + 1 < TERRA_NIC_NAME_MAX && src[i]; ++i)
            dest[i] = src[i];
    }
    dest[i] = 0;
}

static void nic_copy_mac(uint8_t *dest, const uint8_t *src) {
    for (uint32_t i = 0; i < 6; ++i)
        dest[i] = src[i];
}

static terra_nic_record_t *nic_find(terra_nic_id_t id) {
    for (uint32_t i = 0; i < TERRA_NIC_MAX_DEVICES; ++i) {
        if (nics[i].info.active && nics[i].info.id == id)
            return &nics[i];
    }
    return 0;
}

static terra_net_result_t nic_net_tx(terra_netif_id_t netif, const void *packet, size_t length, void *context) {
    terra_nic_record_t *record = (terra_nic_record_t *)context;
    (void)netif;
    if (!record || !record->transmit)
        return TERRA_NET_ERR_INVALID;
    if (record->transmit(record->info.id, packet, length, record->context) < 0)
        return TERRA_NET_ERR_INVALID;
    record->info.tx_packets++;
    return TERRA_NET_OK;
}

static int nic_driver_init(const terra_boot_info_t *boot_info) {
    (void)boot_info;
    return nic_ready ? 0 : -1;
}

void nic_init(const terra_boot_info_t *boot_info) {
    (void)boot_info;
    for (uint32_t i = 0; i < TERRA_NIC_MAX_DEVICES; ++i)
        nics[i].info.active = 0;
    next_nic_id = 1;
    nic_ready = 1;
    terra_driver_t driver = {"nic-core", TERRA_DRIVER_NETWORK, nic_driver_init, nic_shutdown, 1};
    drivers_register(&driver);
}

void nic_shutdown(void) {
    for (uint32_t i = 0; i < TERRA_NIC_MAX_DEVICES; ++i) {
        if (nics[i].info.active)
            netif_unregister(nics[i].info.netif);
        nics[i].info.active = 0;
    }
    nic_ready = 0;
}

int nic_register_device(const terra_nic_config_t *config, terra_nic_id_t *out_id) {
    if (!nic_ready || !config || !config->name || !config->transmit || !out_id)
        return -1;
    for (uint32_t i = 0; i < TERRA_NIC_MAX_DEVICES; ++i) {
        if (nics[i].info.active)
            continue;
        nics[i].info.id = next_nic_id++;
        nic_copy_name(nics[i].info.name, config->name);
        nic_copy_mac(nics[i].info.mac, config->mac);
        nics[i].info.mtu = config->mtu ? config->mtu : 1500u;
        nics[i].info.rx_packets = 0;
        nics[i].info.tx_packets = 0;
        nics[i].info.active = 1;
        nics[i].transmit = config->transmit;
        nics[i].context = config->context;

        terra_netif_config_t netif = {
            .name = config->name,
            .mtu = nics[i].info.mtu,
            .hwaddr_len = 6,
            .transmit = nic_net_tx,
            .context = &nics[i],
        };
        nic_copy_mac(netif.hwaddr, config->mac);
        if (netif_register(&netif, &nics[i].info.netif) != TERRA_NET_OK) {
            nics[i].info.active = 0;
            return -1;
        }
        netif_set_status(nics[i].info.netif, TERRA_NET_UP);
        *out_id = nics[i].info.id;
        return 0;
    }
    return -1;
}

int nic_unregister_device(terra_nic_id_t id) {
    terra_nic_record_t *record = nic_find(id);
    if (!record)
        return -1;
    netif_unregister(record->info.netif);
    record->info.active = 0;
    return 0;
}

int nic_info(terra_nic_id_t id, terra_nic_info_t *out_info) {
    terra_nic_record_t *record = nic_find(id);
    if (!record || !out_info)
        return -1;
    *out_info = record->info;
    return 0;
}

int nic_receive(terra_nic_id_t id, uint16_t protocol, const void *packet, size_t bytes) {
    terra_nic_record_t *record = nic_find(id);
    if (!record || !packet || !bytes)
        return -1;
    if (net_receive_packet(record->info.netif, protocol, packet, bytes) != TERRA_NET_OK)
        return -1;
    record->info.rx_packets++;
    return 0;
}
