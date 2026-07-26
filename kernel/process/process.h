#ifndef TERRARIUM_PROCESS_H
#define TERRARIUM_PROCESS_H

#include <bootloader.h>
#include <stddef.h>
#include <stdint.h>
#include <status.h>

#ifdef __cplusplus
extern "C" {
#endif

#define TERRA_PROCESS_MAX 128u
#define TERRA_PROCESS_NAME_MAX 32u
#define TERRA_KERNEL_PID 1u

typedef enum terra_process_state {
    TERRA_PROCESS_EMPTY = 0,
    TERRA_PROCESS_READY,
    TERRA_PROCESS_RUNNING,
    TERRA_PROCESS_WAITING,
    TERRA_PROCESS_EXITED,
} terra_process_state_t;

typedef struct terra_process_info {
    uint32_t pid;
    uint32_t parent_pid;
    terra_process_state_t state;
    int exit_status;
    char name[TERRA_PROCESS_NAME_MAX];
} terra_process_info_t;

void process_init(const terra_boot_info_t *boot_info);
int process_is_ready(void);
void process_shutdown(void);
uint32_t process_current_pid(void);
int process_create_kernel(const char *name, uint32_t parent_pid, uint32_t *out_pid);
int process_exit(uint32_t pid, int status);
int process_wait(uint32_t parent_pid, uint32_t child_pid, int *out_status);
int process_get(uint32_t pid, terra_process_info_t *out_info);
uint32_t process_count(void);

#ifdef __cplusplus
}
#endif

#endif