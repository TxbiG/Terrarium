#include "rtl8139.h"

#include "../drivers.h"
#include "nic_controller.h"

static int rtl8139_ready;

static int rtl8139_driver_init(const terra_boot_info_t *boot_info) {
    (void)boot_info;
    return rtl8139_ready ? 0 : -1;
}

static int rtl8139_probe(uint16_t vendor_id,
                         uint16_t device_id,
                         uintptr_t mmio_base,
                         uint16_t io_base,
                         void *context,
                         terra_nic_id_t *out_id) {
    (void)vendor_id;
    (void)device_id;
    (void)mmio_base;
    (void)io_base;
    (void)context;
    (void)out_id;
    return -1;
}

void rtl8139_init(const terra_boot_info_t *boot_info) {
    (void)boot_info;
    rtl8139_ready = 1;
    terra_driver_t driver = {"rtl8139", TERRA_DRIVER_NETWORK, rtl8139_driver_init, rtl8139_shutdown, 1};
    drivers_register(&driver);
    terra_nic_driver_probe_t probe = {"rtl8139", 0x10ecu, 0x8139u, rtl8139_probe, 0};
    nic_controller_register_driver(&probe);
}

void rtl8139_shutdown(void) {
    rtl8139_ready = 0;
}

int rtl8139_register_device(uint16_t io_base,
                            const uint8_t mac[6],
                            terra_nic_tx_fn_t transmit,
                            void *context,
                            terra_nic_id_t *out_id) {
    (void)io_base;
    if (!mac || !transmit || !out_id)
        return -1;
    terra_nic_config_t config = {
        .name = "rtl8139",
        .mtu = 1500,
        .transmit = transmit,
        .context = context,
    };
    for (uint32_t i = 0; i < 6; ++i)
        config.mac[i] = mac[i];
    return nic_register_device(&config, out_id);
}
