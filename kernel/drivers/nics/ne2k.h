#ifndef TERRARIUM_DRIVERS_NE2K_H
#define TERRARIUM_DRIVERS_NE2K_H

#include <bootloader.h>
#include <stdint.h>
#include "nic.h"

void ne2k_init(const terra_boot_info_t *boot_info);
void ne2k_shutdown(void);
int ne2k_register_device(uint16_t io_base,
                         const uint8_t mac[6],
                         terra_nic_tx_fn_t transmit,
                         void *context,
                         terra_nic_id_t *out_id);

#endif
