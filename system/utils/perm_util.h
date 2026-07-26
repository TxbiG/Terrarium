#ifndef TERRARIUM_SYSTEM_UTILS_PERM_UTIL_H
#define TERRARIUM_SYSTEM_UTILS_PERM_UTIL_H

#include <stdint.h>

int terra_perm_parse(const char *text, uint32_t *out_mode);
int terra_perm_format(uint32_t mode, char *out_text, unsigned long out_size);

#endif
