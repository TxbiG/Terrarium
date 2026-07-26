#ifndef TERRARIUM_DRIVERS_INPUT_H
#define TERRARIUM_DRIVERS_INPUT_H

#include <bootloader.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define TERRA_INPUT_MAX_DEVICES 32u
#define TERRA_INPUT_QUEUE_SIZE 128u
#define TERRA_INPUT_NAME_MAX 32u

typedef uint32_t terra_input_id_t;

typedef enum terra_input_type {
    TERRA_INPUT_KEYBOARD = 1,
    TERRA_INPUT_MOUSE = 2,
    TERRA_INPUT_TOUCH = 3,
    TERRA_INPUT_CONTROLLER = 4,
    TERRA_INPUT_HAPTIC = 5
} terra_input_type_t;

typedef struct terra_input_event {
    terra_input_id_t device;
    terra_input_type_t type;
    uint32_t code;
    int32_t value;
    uint64_t timestamp_ns;
} terra_input_event_t;

typedef struct terra_input_config {
    const char *name;
    terra_input_type_t type;
} terra_input_config_t;

typedef struct terra_input_info {
    terra_input_id_t id;
    char name[TERRA_INPUT_NAME_MAX];
    terra_input_type_t type;
    uint64_t events_sent;
    int active;
} terra_input_info_t;

void input_init(const terra_boot_info_t *boot_info);
void input_shutdown(void);
int input_register_device(const terra_input_config_t *config, terra_input_id_t *out_id);
int input_unregister_device(terra_input_id_t id);
int input_info(terra_input_id_t id, terra_input_info_t *out_info);
int input_emit(const terra_input_event_t *event);
int input_read(terra_input_event_t *out_event);
size_t input_pending(void);

#ifdef __cplusplus
}
#endif

#endif
