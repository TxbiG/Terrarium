#ifndef TERRARIUM_SERVICE_MANAGER_H
#define TERRARIUM_SERVICE_MANAGER_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#define TERRA_SERVICE_NAME_MAX 48u
#define TERRA_SERVICE_EXEC_MAX 96u
#define TERRA_SERVICE_ARGS_MAX 128u
#define TERRA_SERVICE_DESCRIPTION_MAX 128u
#define TERRA_SERVICE_MAX_DEPS 8u
#define TERRA_SERVICE_MAX_UNITS 32u

typedef enum terra_service_state {
    TERRA_SERVICE_STOPPED = 0,
    TERRA_SERVICE_STARTING,
    TERRA_SERVICE_RUNNING,
    TERRA_SERVICE_FAILED,
    TERRA_SERVICE_BLOCKED
} terra_service_state_t;

typedef enum terra_service_restart {
    TERRA_SERVICE_RESTART_NEVER = 0,
    TERRA_SERVICE_RESTART_ON_FAILURE,
    TERRA_SERVICE_RESTART_ALWAYS
} terra_service_restart_t;

typedef struct terra_service_unit {
    const char *name;
    const char *exec;
    const char *description;
    const char *args;
    const char *deps[TERRA_SERVICE_MAX_DEPS];
    terra_service_restart_t restart;
    int required;
} terra_service_unit_t;

void terra_service_manager_reset(void);
int terra_service_manager_load_defaults(void);
int terra_service_manager_load_manifest(const char *manifest_path);
size_t terra_service_manager_count(void);
const terra_service_unit_t *terra_service_manager_unit(size_t index);
terra_service_state_t terra_service_manager_state(size_t index);
int terra_service_manager_start_all(int dry_run);
int terra_service_manager_start(const char *name, int dry_run);
int terra_service_manager_stop(const char *name, int dry_run);
int terra_service_manager_restart(const char *name, int dry_run);
int terra_service_manager_status(void);
int terra_service_manager_list(void);

#ifdef __cplusplus
}
#endif

#endif