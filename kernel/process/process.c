#include "process.h"

static terra_process_info_t processes[TERRA_PROCESS_MAX];
static uint32_t next_pid;
static uint32_t current_pid;
static int process_ready;

static void process_zero(void *ptr, size_t size) {
    unsigned char *bytes = (unsigned char *)ptr;
    while (size--)
        *bytes++ = 0;
}

static void process_copy_name(char *dst, const char *src) {
    uint32_t i = 0;
    if (!src)
        src = "process";
    while (src[i] && i + 1 < TERRA_PROCESS_NAME_MAX) {
        dst[i] = src[i];
        ++i;
    }
    dst[i] = '\0';
}

static terra_process_info_t *process_slot(uint32_t pid) {
    for (uint32_t i = 0; i < TERRA_PROCESS_MAX; ++i) {
        if (processes[i].state != TERRA_PROCESS_EMPTY && processes[i].pid == pid)
            return &processes[i];
    }
    return 0;
}

void process_init(const terra_boot_info_t *boot_info) {
    (void)boot_info;
    for (uint32_t i = 0; i < TERRA_PROCESS_MAX; ++i)
        process_zero(&processes[i], sizeof(processes[i]));

    next_pid = TERRA_KERNEL_PID + 1u;
    current_pid = TERRA_KERNEL_PID;
    processes[0].pid = TERRA_KERNEL_PID;
    processes[0].parent_pid = 0;
    processes[0].state = TERRA_PROCESS_RUNNING;
    processes[0].exit_status = 0;
    process_copy_name(processes[0].name, "kernel");
    process_ready = 1;
}

int process_is_ready(void) {
    return process_ready;
}

void process_shutdown(void) {
    for (uint32_t i = 0; i < TERRA_PROCESS_MAX; ++i)
        process_zero(&processes[i], sizeof(processes[i]));
    current_pid = 0;
    next_pid = TERRA_KERNEL_PID;
    process_ready = 0;
}

uint32_t process_current_pid(void) {
    return current_pid;
}

int process_create_kernel(const char *name, uint32_t parent_pid, uint32_t *out_pid) {
    if (!process_ready || !out_pid)
        return TERRA_STATUS_INVAL;
    if (parent_pid && !process_slot(parent_pid))
        return TERRA_STATUS_NOENT;

    for (uint32_t i = 0; i < TERRA_PROCESS_MAX; ++i) {
        if (processes[i].state == TERRA_PROCESS_EMPTY) {
            processes[i].pid = next_pid++;
            processes[i].parent_pid = parent_pid ? parent_pid : current_pid;
            processes[i].state = TERRA_PROCESS_READY;
            processes[i].exit_status = 0;
            process_copy_name(processes[i].name, name);
            *out_pid = processes[i].pid;
            return TERRA_STATUS_OK;
        }
    }

    return TERRA_STATUS_NOSPC;
}

int process_exit(uint32_t pid, int status) {
    terra_process_info_t *process = process_slot(pid);
    if (!process)
        return TERRA_STATUS_NOENT;
    if (pid == TERRA_KERNEL_PID)
        return TERRA_STATUS_PERM;

    process->state = TERRA_PROCESS_EXITED;
    process->exit_status = status;
    return TERRA_STATUS_OK;
}

int process_wait(uint32_t parent_pid, uint32_t child_pid, int *out_status) {
    terra_process_info_t *child = process_slot(child_pid);
    if (!child || child->parent_pid != parent_pid)
        return TERRA_STATUS_NOENT;
    if (child->state != TERRA_PROCESS_EXITED)
        return TERRA_STATUS_NOTREADY;

    if (out_status)
        *out_status = child->exit_status;
    process_zero(child, sizeof(*child));
    return TERRA_STATUS_OK;
}

int process_get(uint32_t pid, terra_process_info_t *out_info) {
    terra_process_info_t *process = process_slot(pid);
    if (!out_info)
        return TERRA_STATUS_INVAL;
    if (!process)
        return TERRA_STATUS_NOENT;

    *out_info = *process;
    return TERRA_STATUS_OK;
}

uint32_t process_count(void) {
    uint32_t count = 0;
    for (uint32_t i = 0; i < TERRA_PROCESS_MAX; ++i) {
        if (processes[i].state != TERRA_PROCESS_EMPTY)
            ++count;
    }
    return count;
}