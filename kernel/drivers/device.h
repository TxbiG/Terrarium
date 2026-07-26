#ifndef TERRARIUM_DEVICE_H
#define TERRARIUM_DEVICE_H

#include "drivers.h"

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define TERRA_DEVICE_NAME_MAX 64u
#define TERRA_DEVICE_PATH_MAX 128u
#define TERRA_DEVICE_MAX_RESOURCES 8u

typedef enum terra_device_bus {
    TERRA_DEVICE_BUS_INTERNAL = 0,
    TERRA_DEVICE_BUS_PCI,
    TERRA_DEVICE_BUS_USB,
    TERRA_DEVICE_BUS_PLATFORM,
    TERRA_DEVICE_BUS_VIRTIO
} terra_device_bus_t;

typedef enum terra_device_resource_type {
    TERRA_DEVICE_RESOURCE_MMIO = 0,
    TERRA_DEVICE_RESOURCE_IOPORT,
    TERRA_DEVICE_RESOURCE_IRQ,
    TERRA_DEVICE_RESOURCE_DMA
} terra_device_resource_type_t;

typedef struct terra_device_resource {
    terra_device_resource_type_t type;
    uint64_t base;
    uint64_t length;
    uint32_t flags;
} terra_device_resource_t;

typedef struct terra_device_desc {
    const char *name;
    const char *path;
    const char *driver;
    terra_driver_class_t device_class;
    terra_device_bus_t bus;
    uint16_t vendor_id;
    uint16_t device_id;
    const terra_device_resource_t *resources;
    uint32_t resource_count;
} terra_device_desc_t;

typedef struct terra_device {
    uint32_t id;
    char name[TERRA_DEVICE_NAME_MAX];
    char path[TERRA_DEVICE_PATH_MAX];
    char driver[TERRA_DEVICE_NAME_MAX];
    terra_driver_class_t device_class;
    terra_device_bus_t bus;
    uint16_t vendor_id;
    uint16_t device_id;
    uint32_t resource_count;
    terra_device_resource_t resources[TERRA_DEVICE_MAX_RESOURCES];
    int online;
} terra_device_t;

void terra_device_model_init(void);
int terra_device_publish(const terra_device_desc_t *desc);
int terra_device_unpublish(const char *name);
size_t terra_device_count(void);
const terra_device_t *terra_device_get(size_t index);
const terra_device_t *terra_device_find(const char *name);
int terra_device_set_online(const char *name, int online);

#ifdef __cplusplus
}
#endif

#endif
