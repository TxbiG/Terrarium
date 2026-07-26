#ifndef DRIVERS_BUS_VIRTIO_H
#define DRIVERS_BUS_VIRTIO_H

#include <bootloader.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

typedef enum virtio_device_type {
    VIRTIO_DEVICE_UNKNOWN = 0,
    VIRTIO_DEVICE_NET = 1,
    VIRTIO_DEVICE_BLOCK = 2,
    VIRTIO_DEVICE_CONSOLE = 3,
    VIRTIO_DEVICE_GPU = 16,
} virtio_device_type_t;

typedef struct virtio_device {
    virtio_device_type_t type;
    uint32_t device_id;
    uint32_t vendor_id;
    uintptr_t mmio_base;
} virtio_device_t;

void virtio_init(const terra_boot_info_t *boot_info);
int virtio_is_ready(void);
size_t virtio_device_count(void);
const virtio_device_t *virtio_get_device(size_t index);
int virtio_register_device(const virtio_device_t *device);

#ifdef __cplusplus
}
#endif  // __cplusplus

#endif // DRIVERS_BUS_VIRTIO_H
