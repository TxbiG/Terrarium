#ifndef TERRARIUM_SYSTEM_RUNTIME_SERVICE_H
#define TERRARIUM_SYSTEM_RUNTIME_SERVICE_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define TERRA_SERVICE_NAME_MAX 48u
#define TERRA_SERVICE_DESCRIPTION_MAX 128u

typedef enum terra_service_state {
    TERRA_SERVICE_STOPPED = 0,
    TERRA_SERVICE_STARTING,
    TERRA_SERVICE_RUNNING,
    TERRA_SERVICE_FAILED,
    TERRA_SERVICE_BLOCKED
} terra_service_state_t;

typedef struct terra_service_info {
    char name[TERRA_SERVICE_NAME_MAX];
    terra_service_state_t state;
    uint32_t required;
    char description[TERRA_SERVICE_DESCRIPTION_MAX];
} terra_service_info_t;

int terra_service_start(const char *name);
int terra_service_stop(const char *name);
int terra_service_status(const char *name, terra_service_info_t *out_info);
int terra_service_list(terra_service_info_t *out_services, uint32_t capacity, uint32_t *out_count);

#ifdef __cplusplus
}
#endif

#endif