#ifndef TERRARIUM_DRIVERS_H
#define TERRARIUM_DRIVERS_H

#include <bootloader.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum terra_driver_class {
    TERRA_DRIVER_BUS = 0,
    TERRA_DRIVER_STORAGE,
    TERRA_DRIVER_INPUT,
    TERRA_DRIVER_DISPLAY,
    TERRA_DRIVER_NETWORK,
    TERRA_DRIVER_AUDIO,
    TERRA_DRIVER_MISC,
} terra_driver_class_t;

typedef enum terra_driver_state {
    TERRA_DRIVER_EMPTY = 0,
    TERRA_DRIVER_REGISTERED,
    TERRA_DRIVER_READY,
    TERRA_DRIVER_FAILED,
    TERRA_DRIVER_STOPPED
} terra_driver_state_t;

typedef struct terra_driver {
    const char *name;
    terra_driver_class_t driver_class;
    int (*init)(const terra_boot_info_t *boot_info);
    void (*shutdown)(void);
    int ready;
} terra_driver_t;

void drivers_init(const terra_boot_info_t *boot_info);
int drivers_is_ready(void);
void drivers_shutdown(void);
int drivers_register(const terra_driver_t *driver);
int drivers_unregister(const char *name);
size_t drivers_count(void);
const terra_driver_t *drivers_get(size_t index);
const terra_driver_t *drivers_find(const char *name);
terra_driver_state_t drivers_state(size_t index);

#ifdef __cplusplus
}
#endif

#endif
