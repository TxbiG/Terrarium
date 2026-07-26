#include "nic_controller.h"

#include "../drivers.h"

static terra_nic_driver_probe_t nic_drivers[TERRA_NIC_CONTROLLER_MAX_DRIVERS];
static uint32_t nic_driver_count;
static int nic_controller_ready;

static int nic_name_eq(const char *a, const char *b) {
    if (!a || !b)
        return 0;
    while (*a && *b) {
        if (*a != *b)
            return 0;
        ++a;
        ++b;
    }
    return *a == *b;
}

static int nic_controller_driver_init(const terra_boot_info_t *boot_info) {
    (void)boot_info;
    return nic_controller_ready ? 0 : -1;
}

void nic_controller_init(const terra_boot_info_t *boot_info) {
    (void)boot_info;
    nic_driver_count = 0;
    nic_controller_ready = 1;
    terra_driver_t driver = {"nic-controller", TERRA_DRIVER_NETWORK, nic_controller_driver_init, nic_controller_shutdown, 1};
    drivers_register(&driver);
}

void nic_controller_shutdown(void) {
    nic_driver_count = 0;
    nic_controller_ready = 0;
}

int nic_controller_register_driver(const terra_nic_driver_probe_t *driver) {
    if (!nic_controller_ready || !driver || !driver->name || !driver->probe)
        return -1;
    if (nic_driver_count >= TERRA_NIC_CONTROLLER_MAX_DRIVERS)
        return -1;

    for (uint32_t i = 0; i < nic_driver_count; ++i) {
        if (nic_name_eq(nic_drivers[i].name, driver->name))
            return -1;
    }

    nic_drivers[nic_driver_count++] = *driver;
    return 0;
}

int nic_controller_probe(uint16_t vendor_id,
                         uint16_t device_id,
                         uintptr_t mmio_base,
                         uint16_t io_base,
                         terra_nic_id_t *out_id) {
    if (!nic_controller_ready || !out_id)
        return -1;

    for (uint32_t i = 0; i < nic_driver_count; ++i) {
        int vendor_match = nic_drivers[i].vendor_id == vendor_id;
        int device_match = nic_drivers[i].device_id == 0xffffu ||
                           nic_drivers[i].device_id == device_id;
        if (vendor_match && device_match)
            return nic_drivers[i].probe(vendor_id,
                                        device_id,
                                        mmio_base,
                                        io_base,
                                        nic_drivers[i].context,
                                        out_id);
    }

    return -1;
}
