#ifndef TERRARIUM_MM_H
#define TERRARIUM_MM_H

#include <bootloader.h>

#ifdef __cplusplus
extern "C" {
#endif

void mm_init(const terra_boot_info_t *boot_info);
void mm_terminate(void);
int mm_is_ready(void);
size_t mm_total_memory_bytes(void);
size_t mm_available_memory_bytes(void);

#ifdef __cplusplus
}
#endif

#endif
