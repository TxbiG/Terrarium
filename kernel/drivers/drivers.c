#include "drivers.h"
#include "device.h"

#include "bus/PCI.h"
#include "bus/Serial.h"
#include "bus/VirtIO.h"
#include "audio/audio.h"
#include "camera/camera.h"
#include "gpu/gpu.h"
#include "input/input.h"
#include "nics/nic.h"
#include "nics/nic_controller.h"
#include "nics/e1000.h"
#include "nics/ne2k.h"
#include "nics/rtl8139.h"
#include "nics/rtl8169.h"
#include "storage/storage.h"
#include "storage/ata.h"
#include "storage/nvme.h"

#define TERRARIUM_MAX_DRIVERS 64u

static int drivers_ready;
static terra_driver_t driver_table[TERRARIUM_MAX_DRIVERS];
static terra_driver_state_t driver_states[TERRARIUM_MAX_DRIVERS];
static size_t driver_count;


static void publish_driver_device(const terra_driver_t *driver) {
    char path[TERRA_DEVICE_PATH_MAX];
    const char *name;
    size_t i = 0;

    if (!driver || !driver->name)
        return;

    name = driver->name;
    path[i++] = '/';
    path[i++] = 'd';
    path[i++] = 'e';
    path[i++] = 'v';
    path[i++] = '/';
    while (*name && i + 1 < sizeof(path))
        path[i++] = *name++;
    path[i] = '\0';

    terra_device_desc_t desc = {
        .name = driver->name,
        .path = path,
        .driver = driver->name,
        .device_class = driver->driver_class,
        .bus = TERRA_DEVICE_BUS_INTERNAL,
        .vendor_id = 0,
        .device_id = 0,
        .resources = 0,
        .resource_count = 0,
    };

    (void)terra_device_publish(&desc);
}
static int driver_name_equals(const char *left, const char *right) {
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

void drivers_init(const terra_boot_info_t *boot_info) {
    drivers_ready = 0;
    driver_count = 0;
    terra_device_model_init();
    for (size_t i = 0; i < TERRARIUM_MAX_DRIVERS; ++i)
        driver_states[i] = TERRA_DRIVER_EMPTY;

    pci_init(boot_info);
    serial_init(boot_info);
    virtio_init(boot_info);
    audio_init(boot_info);
    camera_init(boot_info);
    gpu_init(boot_info);
    input_init(boot_info);
    nic_init(boot_info);
    storage_init(boot_info);
    nic_controller_init(boot_info);
    e1000_init(boot_info);
    ne2k_init(boot_info);
    rtl8139_init(boot_info);
    rtl8169_init(boot_info);
    ata_init(boot_info);
    nvme_init(boot_info);

    drivers_ready = 1;
}

int drivers_is_ready(void) {
    return drivers_ready;
}

void drivers_shutdown(void) {
    for (size_t i = driver_count; i > 0; --i) {
        size_t index = i - 1;
        if (driver_table[index].shutdown)
            driver_table[index].shutdown();
        driver_states[index] = TERRA_DRIVER_STOPPED;
        driver_table[index].ready = 0;
    }

    driver_count = 0;
    drivers_ready = 0;
}

int drivers_register(const terra_driver_t *driver) {
    if (!driver || !driver->name || driver_count >= TERRARIUM_MAX_DRIVERS)
        return -1;

    if (drivers_find(driver->name))
        return -1;

    driver_table[driver_count] = *driver;
    driver_states[driver_count] = driver->ready ? TERRA_DRIVER_READY
                                                : TERRA_DRIVER_REGISTERED;
    publish_driver_device(driver);
    ++driver_count;
    return 0;
}

int drivers_unregister(const char *name) {
    if (!name)
        return -1;

    for (size_t i = 0; i < driver_count; ++i) {
        if (!driver_name_equals(driver_table[i].name, name))
            continue;

        if (driver_table[i].shutdown)
            driver_table[i].shutdown();
        (void)terra_device_unpublish(driver_table[i].name);

        for (size_t j = i; j + 1 < driver_count; ++j) {
            driver_table[j] = driver_table[j + 1];
            driver_states[j] = driver_states[j + 1];
        }
        --driver_count;
        driver_states[driver_count] = TERRA_DRIVER_EMPTY;
        return 0;
    }

    return -1;
}

size_t drivers_count(void) {
    return driver_count;
}

const terra_driver_t *drivers_get(size_t index) {
    if (index >= driver_count)
        return 0;

    return &driver_table[index];
}

const terra_driver_t *drivers_find(const char *name) {
    for (size_t i = 0; i < driver_count; ++i) {
        if (driver_name_equals(driver_table[i].name, name))
            return &driver_table[i];
    }

    return 0;
}

terra_driver_state_t drivers_state(size_t index) {
    if (index >= driver_count)
        return TERRA_DRIVER_EMPTY;

    return driver_states[index];
}
