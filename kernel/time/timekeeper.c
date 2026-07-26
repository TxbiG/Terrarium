#include "timekeeper.h"

static uint64_t monotonic_ns;
static uint64_t tick_count;
static uint64_t resolution_ns;
static terra_clocksource_t active_clocksource;
static terra_timer_t *timer_slots[TERRA_TIME_MAX_TIMERS];
static int time_ready;

static uint64_t fallback_clock_read(void *context) {
    (void)context;
    return monotonic_ns;
}

static uint64_t scale_ticks_to_ns(uint64_t ticks, uint64_t frequency_hz) {
    if (!frequency_hz)
        return TERRA_STATUS_OK;

    return (ticks / frequency_hz) * 1000000000ull +
           ((ticks % frequency_hz) * 1000000000ull) / frequency_hz;
}

static void sync_from_clocksource(void) {
    if (!active_clocksource.read || !active_clocksource.frequency_hz)
        return;

    uint64_t ticks = active_clocksource.read(active_clocksource.context);
    uint64_t ns = scale_ticks_to_ns(ticks, active_clocksource.frequency_hz);
    if (ns > monotonic_ns)
        monotonic_ns = ns;
}

void time_init(const terra_boot_info_t *boot_info) {
    (void)boot_info;
    monotonic_ns = 0;
    tick_count = 0;
    resolution_ns = 1000000ull;
    active_clocksource.name = "terrarium-fallback";
    active_clocksource.read = fallback_clock_read;
    active_clocksource.context = 0;
    active_clocksource.frequency_hz = 1000000000ull;
    active_clocksource.rating = 1;

    for (uint32_t i = 0; i < TERRA_TIME_MAX_TIMERS; ++i)
        timer_slots[i] = 0;

    time_ready = 1;
}

int time_is_ready(void) {
    return time_ready;
}

void time_shutdown(void) {
    for (uint32_t i = 0; i < TERRA_TIME_MAX_TIMERS; ++i)
        timer_slots[i] = 0;

    time_ready = 0;
}

uint64_t time_ns(void) {
    sync_from_clocksource();
    return monotonic_ns;
}

uint64_t time_ms(void) {
    return time_ns() / 1000000ull;
}

uint64_t time_seconds(void) {
    return time_ns() / 1000000000ull;
}

uint64_t time_ticks(void) {
    return tick_count;
}

uint64_t time_resolution_ns(void) {
    return resolution_ns;
}

void time_tick(uint64_t ns_delta) {
    if (!time_ready)
        return;

    monotonic_ns += ns_delta;
    tick_count++;
    if (ns_delta && (!resolution_ns || ns_delta < resolution_ns))
        resolution_ns = ns_delta;

    time_run_due_timers();
}

int time_register_clocksource(const terra_clocksource_t *clocksource) {
    if (!clocksource || !clocksource->read || !clocksource->frequency_hz)
        return TERRA_STATUS_INVAL;

    if (!active_clocksource.read ||
        clocksource->rating >= active_clocksource.rating) {
        active_clocksource = *clocksource;
        resolution_ns = 1000000000ull / clocksource->frequency_hz;
        if (!resolution_ns)
            resolution_ns = 1;
    }

    return TERRA_STATUS_OK;
}

const terra_clocksource_t *time_active_clocksource(void) {
    return active_clocksource.read ? &active_clocksource : 0;
}

int time_timer_start(terra_timer_t *timer,
                     uint64_t delay_ns,
                     uint64_t interval_ns,
                     terra_timer_callback_t callback,
                     void *context,
                     uint32_t flags) {
    if (!timer || !callback)
        return TERRA_STATUS_INVAL;

    if (timer->active)
        time_timer_cancel(timer);

    uint32_t free_slot = TERRA_TIME_MAX_TIMERS;
    for (uint32_t i = 0; i < TERRA_TIME_MAX_TIMERS; ++i) {
        if (!timer_slots[i]) {
            free_slot = i;
            break;
        }
    }

    if (free_slot == TERRA_TIME_MAX_TIMERS)
        return TERRA_STATUS_NOSPC;

    timer->deadline_ns = time_ns() + delay_ns;
    timer->interval_ns = interval_ns;
    timer->callback = callback;
    timer->context = context;
    timer->flags = flags;
    timer->active = 1;
    timer_slots[free_slot] = timer;
    return TERRA_STATUS_OK;
}

int time_timer_cancel(terra_timer_t *timer) {
    if (!timer)
        return TERRA_STATUS_INVAL;

    for (uint32_t i = 0; i < TERRA_TIME_MAX_TIMERS; ++i) {
        if (timer_slots[i] == timer)
            timer_slots[i] = 0;
    }

    timer->active = 0;
    timer->callback = 0;
    timer->context = 0;
    timer->deadline_ns = 0;
    timer->interval_ns = 0;
    timer->flags = 0;
    return TERRA_STATUS_OK;
}

void time_run_due_timers(void) {
    uint64_t now = time_ns();

    for (uint32_t i = 0; i < TERRA_TIME_MAX_TIMERS; ++i) {
        terra_timer_t *timer = timer_slots[i];
        if (!timer || !timer->active || timer->deadline_ns > now)
            continue;

        terra_timer_callback_t callback = timer->callback;
        void *context = timer->context;

        if ((timer->flags & TERRA_TIMER_PERIODIC) && timer->interval_ns) {
            do {
                timer->deadline_ns += timer->interval_ns;
            } while (timer->deadline_ns <= now);
        } else {
            timer_slots[i] = 0;
            timer->active = 0;
        }

        if (callback)
            callback(context);
    }
}

void time_sleep_ms(uint64_t ms) {
    uint64_t deadline = time_ns() + ms * 1000000ull;
    while (time_ns() < deadline) {
#if defined(__x86_64__) || defined(__i386__)
        __asm__ volatile("pause");
#elif defined(__aarch64__) || defined(__arm__)
        __asm__ volatile("yield");
#elif defined(__riscv)
        __asm__ volatile("nop");
#endif
    }
}
