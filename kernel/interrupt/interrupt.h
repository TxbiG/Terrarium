#ifndef TERRARIUM_INTERRUPT_H
#define TERRARIUM_INTERRUPT_H

#include <bootloader.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define TERRA_INTERRUPT_VECTOR_COUNT 256u
#define TERRA_INTERRUPT_MAX_HANDLERS 512u

typedef enum terra_interrupt_result {
    TERRA_INTERRUPT_UNHANDLED = 0,
    TERRA_INTERRUPT_HANDLED = 1
} terra_interrupt_result_t;

typedef struct terra_interrupt_frame {
    uint64_t vector;
    uint64_t error_code;
    uint64_t instruction_pointer;
    uint64_t code_segment;
    uint64_t flags;
    uint64_t stack_pointer;
    uint64_t stack_segment;
} terra_interrupt_frame_t;

typedef terra_interrupt_result_t (*terra_interrupt_handler_t)(
    const terra_interrupt_frame_t *frame,
    void *context);

typedef struct terra_interrupt_stats {
    uint64_t delivered;
    uint64_t handled;
    uint64_t unhandled;
} terra_interrupt_stats_t;

void interrupt_init(const terra_boot_info_t *boot_info);
int interrupt_is_ready(void);
void interrupt_shutdown(void);
void interrupt_enable(void);
void interrupt_disable(void);
int interrupt_register_handler(uint8_t vector,
                               terra_interrupt_handler_t handler,
                               void *context);
int interrupt_unregister_handler(uint8_t vector,
                                 terra_interrupt_handler_t handler,
                                 void *context);
terra_interrupt_result_t interrupt_dispatch(const terra_interrupt_frame_t *frame);
int interrupt_stats(uint8_t vector, terra_interrupt_stats_t *out);
uint64_t interrupt_unhandled_count(void);
int interrupt_in_handler(void);

#ifdef __cplusplus
}
#endif

#endif
