#include "VirtIO.h"

#include "../drivers.h"

#define VIRTIO_MAX_DEVICES 64u

static virtio_device_t virtio_devices[VIRTIO_MAX_DEVICES];
static size_t virtio_devices_found;
static int virtio_ready;

static int virtio_driver_init(const terra_boot_info_t *boot_info) {
    (void)boot_info;
    return virtio_ready ? 0 : -1;
}

void virtio_init(const terra_boot_info_t *boot_info) {
    (void)boot_info;

    virtio_devices_found = 0;
    virtio_ready = 1;

    terra_driver_t driver = {
        "virtio",
        TERRA_DRIVER_BUS,
        virtio_driver_init,
        0,
        virtio_ready,
    };
    drivers_register(&driver);
}

int virtio_is_ready(void) {
    return virtio_ready;
}

size_t virtio_device_count(void) {
    return virtio_devices_found;
}

const virtio_device_t *virtio_get_device(size_t index) {
    if (index >= virtio_devices_found)
        return 0;

    return &virtio_devices[index];
}

int virtio_register_device(const virtio_device_t *device) {
    if (!device || virtio_devices_found >= VIRTIO_MAX_DEVICES)
        return -1;

    virtio_devices[virtio_devices_found] = *device;
    ++virtio_devices_found;
    return 0;
}
