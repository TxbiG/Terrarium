#ifndef TERRARIUM_SYSTEM_RUNTIME_PROCESS_H
#define TERRARIUM_SYSTEM_RUNTIME_PROCESS_H

#include <stdint.h>

typedef int32_t terra_pid_t;

typedef enum terra_process_state {
    TERRA_PROCESS_NEW = 0,
    TERRA_PROCESS_RUNNING,
    TERRA_PROCESS_SLEEPING,
    TERRA_PROCESS_STOPPED,
    TERRA_PROCESS_ZOMBIE
} terra_process_state_t;

typedef struct terra_process_info {
    terra_pid_t pid;
    terra_pid_t parent_pid;
    terra_process_state_t state;
    char name[64];
} terra_process_info_t;

terra_pid_t terra_process_spawn(const char *path, char *const argv[]);
int terra_process_wait(terra_pid_t pid, int *out_status);
int terra_process_kill(terra_pid_t pid, int signal);
int terra_process_info(terra_pid_t pid, terra_process_info_t *out_info);

#endif
