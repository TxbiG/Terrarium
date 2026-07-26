#ifndef TERRARIUM_SYSTEM_UTILS_DISK_UTIL_H
#define TERRARIUM_SYSTEM_UTILS_DISK_UTIL_H

#include <stdint.h>

int terra_disk_human_size(uint64_t bytes, char *out_text, unsigned long out_size);
int terra_disk_usage(const char *path, uint64_t *out_used, uint64_t *out_total);

#endif
