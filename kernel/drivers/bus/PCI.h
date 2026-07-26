#ifndef DRIVERS_BUS_PCI_H
#define DRIVERS_BUS_PCI_H

// PCI (Peripheral Component Interconnect)

#include <bootloader.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

typedef struct pci_device {
    uint8_t bus;
    uint8_t slot;
    uint8_t function;
    uint16_t vendor_id;
    uint16_t device_id;
    uint8_t class_code;
    uint8_t subclass;
    uint8_t prog_if;
} pci_device_t;

void pci_init(const terra_boot_info_t *boot_info);
int pci_is_ready(void);
size_t pci_device_count(void);
const pci_device_t *pci_get_device(size_t index);
int pci_register_device(const pci_device_t *device);

#ifdef __cplusplus
}
#endif  // __cplusplus

#endif // DRIVERS_BUS_PCI_H
