#include "Serial.h"

#include "../drivers.h"

#define SERIAL_BOOT_BUFFER_SIZE 4096u

static char serial_boot_buffer[SERIAL_BOOT_BUFFER_SIZE];
static size_t serial_boot_buffer_len;
static int serial_ready;

static int serial_driver_init(const terra_boot_info_t *boot_info) {
    (void)boot_info;
    return serial_ready ? 0 : -1;
}

void serial_init(const terra_boot_info_t *boot_info) {
    (void)boot_info;

    serial_boot_buffer_len = 0;
    serial_ready = 1;

    terra_driver_t driver = {
        "serial",
        TERRA_DRIVER_BUS,
        serial_driver_init,
        0,
        serial_ready,
    };
    drivers_register(&driver);
}

int serial_is_ready(void) {
    return serial_ready;
}

void serial_write_char(char value) {
    if (!serial_ready)
        return;

    if (serial_boot_buffer_len < SERIAL_BOOT_BUFFER_SIZE)
        serial_boot_buffer[serial_boot_buffer_len++] = value;
}

void serial_write(const char *text) {
    if (!text)
        return;

    while (*text) {
        serial_write_char(*text);
        ++text;
    }
}

size_t serial_buffer_size(void) {
    return serial_boot_buffer_len;
}

size_t serial_read_buffer(char *out, size_t out_size) {
    if (!out || out_size == 0)
        return 0;

    size_t count = serial_boot_buffer_len < out_size ? serial_boot_buffer_len : out_size;
    for (size_t i = 0; i < count; ++i)
        out[i] = serial_boot_buffer[i];

    return count;
}
