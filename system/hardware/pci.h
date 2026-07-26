#ifndef TERRARIUM_SYSTEM_HARDWARE_PCI_H
#define TERRARIUM_SYSTEM_HARDWARE_PCI_H

#include <stdint.h>

typedef struct terra_pci_device {
    uint16_t vendor_id;
    uint16_t device_id;
    uint8_t bus;
    uint8_t slot;
    uint8_t function;
} terra_pci_device_t;

int terra_pci_list(terra_pci_device_t *devices, unsigned long max_devices);

#endif
