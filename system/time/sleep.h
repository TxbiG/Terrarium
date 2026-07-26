#ifndef TERRARIUM_SYSTEM_TIME_SLEEP_H
#define TERRARIUM_SYSTEM_TIME_SLEEP_H

#include <stdint.h>

int terra_sleep_ms(uint64_t milliseconds);
int terra_sleep_ns(uint64_t nanoseconds);

#endif
