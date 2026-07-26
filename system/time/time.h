#ifndef TERRARIUM_SYSTEM_TIME_TIME_H
#define TERRARIUM_SYSTEM_TIME_TIME_H

#include <stdint.h>

typedef struct terra_time {
    int64_t seconds;
    uint32_t nanoseconds;
} terra_time_t;

int terra_time_now(terra_time_t *out_time);
int terra_time_monotonic(terra_time_t *out_time);

#endif
