#ifndef DRIVERS_BUS_SERIAL_H
#define DRIVERS_BUS_SERIAL_H

#include <bootloader.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

void serial_init(const terra_boot_info_t *boot_info);
int serial_is_ready(void);
void serial_write_char(char value);
void serial_write(const char *text);
size_t serial_buffer_size(void);
size_t serial_read_buffer(char *out, size_t out_size);

#ifdef __cplusplus
}
#endif  // __cplusplus

#endif // DRIVERS_BUS_SERIAL_H
