#ifndef TERRARIUM_SYSTEM_TIME_CLOCK_H
#define TERRARIUM_SYSTEM_TIME_CLOCK_H

#include "time.h"

typedef enum terra_clock_id {
    TERRA_CLOCK_REALTIME = 0,
    TERRA_CLOCK_MONOTONIC,
    TERRA_CLOCK_BOOTTIME
} terra_clock_id_t;

int terra_clock_get(terra_clock_id_t clock, terra_time_t *out_time);
int terra_clock_set(terra_clock_id_t clock, const terra_time_t *time);

#endif
