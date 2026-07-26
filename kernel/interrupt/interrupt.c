#include "interrupt.h"

typedef struct terra_interrupt_handler_entry {
    terra_interrupt_handler_t handler;
    void *context;
    uint16_t next;
    uint8_t vector;
    uint8_t active;
} terra_interrupt_handler_entry_t;

static terra_interrupt_handler_entry_t handler_pool[TERRA_INTERRUPT_MAX_HANDLERS];
static uint16_t vector_heads[TERRA_INTERRUPT_VECTOR_COUNT];
static terra_interrupt_stats_t vector_stats[TERRA_INTERRUPT_VECTOR_COUNT];
static void (*legacy_handlers[TERRA_INTERRUPT_VECTOR_COUNT])(terra_interrupt_frame_t frame);
static uint64_t total_unhandled;
static uint32_t interrupt_nesting;
static int interrupts_ready;
static int interrupts_enabled;

#define TERRA_INTERRUPT_NO_HANDLER 0xffffu

void __attribute__((weak)) arch_interrupt_enable(void) {
#if defined(__x86_64__) || defined(__i386__)
    __asm__ volatile("sti");
#endif
}

void __attribute__((weak)) arch_interrupt_disable(void) {
#if defined(__x86_64__) || defined(__i386__)
    __asm__ volatile("cli");
#endif
}

void __attribute__((weak)) arch_interrupt_ack(uint8_t vector) {
    (void)vector;
}

static void interrupt_clear_tables(void) {
    for (uint32_t i = 0; i < TERRA_INTERRUPT_VECTOR_COUNT; ++i) {
        vector_heads[i] = TERRA_INTERRUPT_NO_HANDLER;
        vector_stats[i].delivered = 0;
        vector_stats[i].handled = 0;
        vector_stats[i].unhandled = 0;
        legacy_handlers[i] = 0;
    }

    for (uint32_t i = 0; i < TERRA_INTERRUPT_MAX_HANDLERS; ++i) {
        handler_pool[i].handler = 0;
        handler_pool[i].context = 0;
        handler_pool[i].next = TERRA_INTERRUPT_NO_HANDLER;
        handler_pool[i].vector = 0;
        handler_pool[i].active = 0;
    }

    total_unhandled = 0;
    interrupt_nesting = 0;
}

static terra_interrupt_result_t legacy_interrupt_adapter(
    const terra_interrupt_frame_t *frame,
    void *context) {
    (void)context;
    if (!frame || frame->vector >= TERRA_INTERRUPT_VECTOR_COUNT)
        return TERRA_INTERRUPT_UNHANDLED;

    void (*handler)(terra_interrupt_frame_t) = legacy_handlers[frame->vector];
    if (!handler)
        return TERRA_INTERRUPT_UNHANDLED;

    handler(*frame);
    return TERRA_INTERRUPT_HANDLED;
}

void interrupt_init(const terra_boot_info_t *boot_info) {
    (void)boot_info;
    interrupt_clear_tables();
    interrupts_enabled = 0;
    interrupts_ready = 1;
}

int interrupt_is_ready(void) {
    return interrupts_ready;
}

void interrupt_shutdown(void) {
    interrupt_disable();
    interrupt_clear_tables();
    interrupts_ready = 0;
}

void interrupt_enable(void) {
    if (!interrupts_ready)
        return;

    arch_interrupt_enable();
    interrupts_enabled = 1;
}

void interrupt_disable(void) {
    arch_interrupt_disable();
    interrupts_enabled = 0;
}

int interrupt_register_handler(uint8_t vector,
                               terra_interrupt_handler_t handler,
                               void *context) {
    if (!handler)
        return -1;

    for (uint32_t i = 0; i < TERRA_INTERRUPT_MAX_HANDLERS; ++i) {
        if (!handler_pool[i].active)
            continue;
        if (handler_pool[i].vector == vector &&
            handler_pool[i].handler == handler &&
            handler_pool[i].context == context)
            return -1;
    }

    for (uint32_t i = 0; i < TERRA_INTERRUPT_MAX_HANDLERS; ++i) {
        if (handler_pool[i].active)
            continue;

        handler_pool[i].handler = handler;
        handler_pool[i].context = context;
        handler_pool[i].vector = vector;
        handler_pool[i].next = vector_heads[vector];
        handler_pool[i].active = 1;
        vector_heads[vector] = (uint16_t)i;
        return 0;
    }

    return -1;
}

int interrupt_unregister_handler(uint8_t vector,
                                 terra_interrupt_handler_t handler,
                                 void *context) {
    if (!handler)
        return -1;

    uint16_t prev = TERRA_INTERRUPT_NO_HANDLER;
    uint16_t current = vector_heads[vector];

    while (current != TERRA_INTERRUPT_NO_HANDLER) {
        terra_interrupt_handler_entry_t *entry = &handler_pool[current];
        if (entry->active && entry->handler == handler && entry->context == context) {
            if (prev == TERRA_INTERRUPT_NO_HANDLER)
                vector_heads[vector] = entry->next;
            else
                handler_pool[prev].next = entry->next;

            entry->handler = 0;
            entry->context = 0;
            entry->next = TERRA_INTERRUPT_NO_HANDLER;
            entry->vector = 0;
            entry->active = 0;
            return 0;
        }

        prev = current;
        current = entry->next;
    }

    return -1;
}

terra_interrupt_result_t interrupt_dispatch(const terra_interrupt_frame_t *frame) {
    if (!interrupts_ready || !frame || frame->vector >= TERRA_INTERRUPT_VECTOR_COUNT)
        return TERRA_INTERRUPT_UNHANDLED;

    uint8_t vector = (uint8_t)frame->vector;
    terra_interrupt_result_t result = TERRA_INTERRUPT_UNHANDLED;

    interrupt_nesting++;
    vector_stats[vector].delivered++;

    uint16_t current = vector_heads[vector];
    while (current != TERRA_INTERRUPT_NO_HANDLER) {
        terra_interrupt_handler_entry_t *entry = &handler_pool[current];
        current = entry->next;

        if (!entry->active || !entry->handler)
            continue;

        if (entry->handler(frame, entry->context) == TERRA_INTERRUPT_HANDLED)
            result = TERRA_INTERRUPT_HANDLED;
    }

    if (result == TERRA_INTERRUPT_HANDLED) {
        vector_stats[vector].handled++;
    } else {
        vector_stats[vector].unhandled++;
        total_unhandled++;
    }

    arch_interrupt_ack(vector);
    interrupt_nesting--;
    return result;
}

int interrupt_stats(uint8_t vector, terra_interrupt_stats_t *out) {
    if (!out)
        return -1;

    *out = vector_stats[vector];
    return 0;
}

uint64_t interrupt_unhandled_count(void) {
    return total_unhandled;
}

int interrupt_in_handler(void) {
    return interrupt_nesting != 0;
}

void register_interrupt_handler(uint8_t vector,
                                void (*handler)(terra_interrupt_frame_t frame)) {
    legacy_handlers[vector] = handler;
    if (handler)
        (void)interrupt_register_handler(vector, legacy_interrupt_adapter, 0);
}
