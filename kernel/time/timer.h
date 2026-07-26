#ifndef TERRARIUM_TIMER_H
#define TERRARIUM_TIMER_H

#include "timekeeper.h"

typedef terra_timer_callback_t timer_callback_t;

static inline void timer_init(void) {
}

static inline void timer_sleep(uint64_t ms) {
    time_sleep_ms(ms);
}

static inline int timer_add(uint64_t ms, timer_callback_t cb, void *arg) {
    static terra_timer_t compatibility_timer;
    return time_timer_start(&compatibility_timer,
                            ms * 1000000ull,
                            0,
                            cb,
                            arg,
                            0);
}

#endif
