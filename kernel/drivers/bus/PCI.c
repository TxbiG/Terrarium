#include "PCI.h"

#include "../drivers.h"

#define PCI_MAX_DEVICES 128u

static pci_device_t pci_devices[PCI_MAX_DEVICES];
static size_t pci_devices_found;
static int pci_ready;

static int pci_driver_init(const terra_boot_info_t *boot_info) {
    (void)boot_info;
    return pci_ready ? 0 : -1;
}

void pci_init(const terra_boot_info_t *boot_info) {
    (void)boot_info;

    pci_devices_found = 0;
    pci_ready = 1;

    terra_driver_t driver = {
        "pci",
        TERRA_DRIVER_BUS,
        pci_driver_init,
        0,
        pci_ready,
    };
    drivers_register(&driver);
}

int pci_is_ready(void) {
    return pci_ready;
}

size_t pci_device_count(void) {
    return pci_devices_found;
}

const pci_device_t *pci_get_device(size_t index) {
    if (index >= pci_devices_found)
        return 0;

    return &pci_devices[index];
}

int pci_register_device(const pci_device_t *device) {
    if (!device || pci_devices_found >= PCI_MAX_DEVICES)
        return -1;

    pci_devices[pci_devices_found] = *device;
    ++pci_devices_found;
    return 0;
}
