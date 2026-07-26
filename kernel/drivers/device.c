#include "device.h"

#include <status.h>

#define TERRA_MAX_DEVICES 128u

static terra_device_t devices[TERRA_MAX_DEVICES];
static size_t device_count;
static uint32_t next_device_id;

static int str_eq(const char *left, const char *right) {
    if (!left || !right)
        return 0;

    while (*left && *right) {
        if (*left != *right)
            return 0;
        ++left;
        ++right;
    }

    return *left == *right;
}

static void copy_text(char *dst, const char *src, size_t cap) {
    size_t i = 0;

    if (!dst || cap == 0)
        return;
    if (!src)
        src = "";

    while (src[i] && i + 1 < cap) {
        dst[i] = src[i];
        ++i;
    }
    dst[i] = '\0';
}

void terra_device_model_init(void) {
    device_count = 0;
    next_device_id = 1;
}

int terra_device_publish(const terra_device_desc_t *desc) {
    terra_device_t *device;
    uint32_t resource_count;

    if (!desc || !desc->name || !desc->path)
        return TERRA_STATUS_INVAL;
    if (device_count >= TERRA_MAX_DEVICES)
        return TERRA_STATUS_NOSPC;
    if (terra_device_find(desc->name))
        return TERRA_STATUS_EXIST;

    resource_count = desc->resource_count;
    if (resource_count > TERRA_DEVICE_MAX_RESOURCES)
        return TERRA_STATUS_INVAL;
    if (resource_count != 0 && !desc->resources)
        return TERRA_STATUS_INVAL;

    device = &devices[device_count++];
    device->id = next_device_id++;
    copy_text(device->name, desc->name, sizeof(device->name));
    copy_text(device->path, desc->path, sizeof(device->path));
    copy_text(device->driver, desc->driver, sizeof(device->driver));
    device->device_class = desc->device_class;
    device->bus = desc->bus;
    device->vendor_id = desc->vendor_id;
    device->device_id = desc->device_id;
    device->resource_count = resource_count;
    device->online = 1;

    for (uint32_t i = 0; i < resource_count; ++i)
        device->resources[i] = desc->resources[i];

    return TERRA_STATUS_OK;
}


int terra_device_unpublish(const char *name) {
    if (!name)
        return TERRA_STATUS_INVAL;

    for (size_t i = 0; i < device_count; ++i) {
        if (!str_eq(devices[i].name, name))
            continue;

        for (size_t j = i; j + 1 < device_count; ++j)
            devices[j] = devices[j + 1];

        --device_count;
        return TERRA_STATUS_OK;
    }

    return TERRA_STATUS_NOENT;
}
size_t terra_device_count(void) {
    return device_count;
}

const terra_device_t *terra_device_get(size_t index) {
    if (index >= device_count)
        return 0;
    return &devices[index];
}

const terra_device_t *terra_device_find(const char *name) {
    if (!name)
        return 0;

    for (size_t i = 0; i < device_count; ++i) {
        if (str_eq(devices[i].name, name))
            return &devices[i];
    }

    return 0;
}

int terra_device_set_online(const char *name, int online) {
    if (!name)
        return TERRA_STATUS_INVAL;

    for (size_t i = 0; i < device_count; ++i) {
        if (!str_eq(devices[i].name, name))
            continue;

        devices[i].online = online ? 1 : 0;
        return TERRA_STATUS_OK;
    }

    return TERRA_STATUS_NOENT;
}