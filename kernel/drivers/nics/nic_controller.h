#ifndef TERRARIUM_DRIVERS_NIC_CONTROLLER_H
#define TERRARIUM_DRIVERS_NIC_CONTROLLER_H

#include <bootloader.h>
#include <stddef.h>
#include <stdint.h>
#include "nic.h"

#ifdef __cplusplus
extern "C" {
#endif

#define TERRA_NIC_CONTROLLER_MAX_DRIVERS 16u

typedef int (*terra_nic_probe_fn_t)(uint16_t vendor_id,
                                    uint16_t device_id,
                                    uintptr_t mmio_base,
                                    uint16_t io_base,
                                    void *context,
                                    terra_nic_id_t *out_id);

typedef struct terra_nic_driver_probe {
    const char *name;
    uint16_t vendor_id;
    uint16_t device_id;
    terra_nic_probe_fn_t probe;
    void *context;
} terra_nic_driver_probe_t;

void nic_controller_init(const terra_boot_info_t *boot_info);
void nic_controller_shutdown(void);
int nic_controller_register_driver(const terra_nic_driver_probe_t *driver);
int nic_controller_probe(uint16_t vendor_id,
                         uint16_t device_id,
                         uintptr_t mmio_base,
                         uint16_t io_base,
                         terra_nic_id_t *out_id);

#ifdef __cplusplus
}
#endif

#endif
