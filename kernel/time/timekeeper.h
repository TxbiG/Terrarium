#ifndef TERRARIUM_TIMEKEEPER_H
#define TERRARIUM_TIMEKEEPER_H

#include <bootloader.h>
#include <status.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void time_init(const terra_boot_info_t *boot_info);
int time_is_ready(void);
void time_shutdown(void);
uint64_t time_ns(void);
uint64_t time_ms(void);
uint64_t time_seconds(void);
uint64_t time_ticks(void);
uint64_t time_resolution_ns(void);
void time_tick(uint64_t ns_delta);

typedef uint64_t (*terra_clock_read_fn_t)(void *context);

typedef struct terra_clocksource {
    const char *name;
    terra_clock_read_fn_t read;
    void *context;
    uint64_t frequency_hz;
    uint64_t rating;
} terra_clocksource_t;

typedef void (*terra_timer_callback_t)(void *context);

typedef struct terra_timer {
    uint64_t deadline_ns;
    uint64_t interval_ns;
    terra_timer_callback_t callback;
    void *context;
    uint32_t flags;
    uint32_t active;
} terra_timer_t;

#define TERRA_TIMER_PERIODIC 0x1u
#define TERRA_TIME_MAX_TIMERS 64u

int time_register_clocksource(const terra_clocksource_t *clocksource);
const terra_clocksource_t *time_active_clocksource(void);
int time_timer_start(terra_timer_t *timer,
                     uint64_t delay_ns,
                     uint64_t interval_ns,
                     terra_timer_callback_t callback,
                     void *context,
                     uint32_t flags);
int time_timer_cancel(terra_timer_t *timer);
void time_run_due_timers(void);
void time_sleep_ms(uint64_t ms);

#ifdef __cplusplus
}
#endif

#endif
