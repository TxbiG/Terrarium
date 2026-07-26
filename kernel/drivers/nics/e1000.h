#ifndef TERRARIUM_DRIVERS_E1000_H
#define TERRARIUM_DRIVERS_E1000_H

#include <bootloader.h>
#include <stdint.h>
#include "nic.h"

void e1000_init(const terra_boot_info_t *boot_info);
void e1000_shutdown(void);
int e1000_register_device(uintptr_t mmio_base,
                          uint16_t io_base,
                          const uint8_t mac[6],
                          terra_nic_tx_fn_t transmit,
                          void *context,
                          terra_nic_id_t *out_id);

#endif
