#ifndef TERRARIUM_DRIVERS_RTL8139_H
#define TERRARIUM_DRIVERS_RTL8139_H

#include <bootloader.h>
#include <stdint.h>
#include "nic.h"

void rtl8139_init(const terra_boot_info_t *boot_info);
void rtl8139_shutdown(void);
int rtl8139_register_device(uint16_t io_base,
                            const uint8_t mac[6],
                            terra_nic_tx_fn_t transmit,
                            void *context,
                            terra_nic_id_t *out_id);

#endif
