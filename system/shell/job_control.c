#include "job_control.h"
#include "../status.h"

int terra_shell_job_background(int pid) {
    (void)pid;
    return TERRA_STATUS_NOSYS;
}

int terra_shell_job_foreground(int pid) {
    (void)pid;
    return TERRA_STATUS_NOSYS;
}

int terra_shell_job_list(void) {
    return TERRA_STATUS_NOSYS;
}
