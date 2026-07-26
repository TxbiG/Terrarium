#ifndef TERRARIUM_SYSTEM_HARDWARE_CPU_H
#define TERRARIUM_SYSTEM_HARDWARE_CPU_H

#include <stdint.h>

typedef struct terra_cpu_info {
    uint32_t id;
    uint32_t mhz;
    char vendor[32];
    char model[64];
} terra_cpu_info_t;

int terra_cpu_count(void);
int terra_cpu_info(uint32_t id, terra_cpu_info_t *out_info);

#endif
