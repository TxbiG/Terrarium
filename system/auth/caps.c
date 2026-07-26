#include "caps.h"

int terra_caps_has(terra_capset_t caps, enum terra_capability capability) {
    return (caps & (terra_capset_t)capability) == (terra_capset_t)capability;
}

terra_capset_t terra_caps_grant(terra_capset_t caps, enum terra_capability capability) {
    return caps | (terra_capset_t)capability;
}

terra_capset_t terra_caps_drop(terra_capset_t caps, enum terra_capability capability) {
    return caps & ~(terra_capset_t)capability;
}
