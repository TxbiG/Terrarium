#include "input.h"

#include "../drivers.h"

static terra_input_info_t input_devices[TERRA_INPUT_MAX_DEVICES];
static terra_input_event_t input_queue[TERRA_INPUT_QUEUE_SIZE];
static uint32_t input_head;
static uint32_t input_tail;
static size_t input_count_value;
static terra_input_id_t next_input_id;
static int input_ready;

static void input_copy_name(char *dest, const char *src) {
    uint32_t i = 0;
    if (src) {
        for (; i + 1 < TERRA_INPUT_NAME_MAX && src[i]; ++i)
            dest[i] = src[i];
    }
    dest[i] = 0;
}

static terra_input_info_t *input_find(terra_input_id_t id) {
    for (uint32_t i = 0; i < TERRA_INPUT_MAX_DEVICES; ++i) {
        if (input_devices[i].active && input_devices[i].id == id)
            return &input_devices[i];
    }
    return 0;
}

static int input_driver_init(const terra_boot_info_t *boot_info) {
    (void)boot_info;
    return input_ready ? 0 : -1;
}

void input_init(const terra_boot_info_t *boot_info) {
    (void)boot_info;
    for (uint32_t i = 0; i < TERRA_INPUT_MAX_DEVICES; ++i)
        input_devices[i].active = 0;
    input_head = 0;
    input_tail = 0;
    input_count_value = 0;
    next_input_id = 1;
    input_ready = 1;
    terra_driver_t driver = {"input-core", TERRA_DRIVER_INPUT, input_driver_init, input_shutdown, 1};
    drivers_register(&driver);
}

void input_shutdown(void) {
    for (uint32_t i = 0; i < TERRA_INPUT_MAX_DEVICES; ++i)
        input_devices[i].active = 0;
    input_count_value = 0;
    input_ready = 0;
}

int input_register_device(const terra_input_config_t *config, terra_input_id_t *out_id) {
    if (!input_ready || !config || !config->name || !out_id)
        return -1;
    for (uint32_t i = 0; i < TERRA_INPUT_MAX_DEVICES; ++i) {
        if (input_devices[i].active)
            continue;
        input_devices[i].id = next_input_id++;
        input_copy_name(input_devices[i].name, config->name);
        input_devices[i].type = config->type;
        input_devices[i].events_sent = 0;
        input_devices[i].active = 1;
        *out_id = input_devices[i].id;
        return 0;
    }
    return -1;
}

int input_unregister_device(terra_input_id_t id) {
    terra_input_info_t *info = input_find(id);
    if (!info)
        return -1;
    info->active = 0;
    return 0;
}

int input_info(terra_input_id_t id, terra_input_info_t *out_info) {
    terra_input_info_t *info = input_find(id);
    if (!info || !out_info)
        return -1;
    *out_info = *info;
    return 0;
}

int input_emit(const terra_input_event_t *event) {
    terra_input_info_t *info;
    if (!input_ready || !event)
        return -1;
    info = input_find(event->device);
    if (!info)
        return -1;
    if (input_count_value == TERRA_INPUT_QUEUE_SIZE)
        return -1;
    input_queue[input_tail] = *event;
    input_tail = (input_tail + 1u) % TERRA_INPUT_QUEUE_SIZE;
    input_count_value++;
    info->events_sent++;
    return 0;
}

int input_read(terra_input_event_t *out_event) {
    if (!out_event || !input_count_value)
        return -1;
    *out_event = input_queue[input_head];
    input_head = (input_head + 1u) % TERRA_INPUT_QUEUE_SIZE;
    input_count_value--;
    return 0;
}

size_t input_pending(void) {
    return input_count_value;
}
