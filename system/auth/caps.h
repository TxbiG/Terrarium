#ifndef TERRARIUM_SYSTEM_AUTH_CAPS_H
#define TERRARIUM_SYSTEM_AUTH_CAPS_H

#include <stdint.h>

typedef uint64_t terra_capset_t;

enum terra_capability {
    TERRA_CAP_SYS_ADMIN = 1ull << 0,
    TERRA_CAP_NET_ADMIN = 1ull << 1,
    TERRA_CAP_FS_ADMIN = 1ull << 2,
    TERRA_CAP_POWER = 1ull << 3,
    TERRA_CAP_DEBUG = 1ull << 4
};

int terra_caps_has(terra_capset_t caps, enum terra_capability capability);
terra_capset_t terra_caps_grant(terra_capset_t caps, enum terra_capability capability);
terra_capset_t terra_caps_drop(terra_capset_t caps, enum terra_capability capability);

#endif
