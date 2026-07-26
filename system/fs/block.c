#include "block.h"
#include "../status.h"

int terra_block_list(terra_block_device_t *devices, unsigned long max_devices) {
    (void)devices;
    (void)max_devices;
    return TERRA_STATUS_NOSYS;
}

int terra_block_open(const char *name) {
    (void)name;
    return TERRA_STATUS_NOSYS;
}

int terra_block_close(int handle) {
    (void)handle;
    return TERRA_STATUS_NOSYS;
}
