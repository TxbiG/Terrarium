#include "shell.h"
#include "../status.h"

int terra_shell_run(void) {
    return TERRA_STATUS_NOSYS;
}

int terra_shell_execute_line(const char *line) {
    (void)line;
    return TERRA_STATUS_NOSYS;
}

int terra_shell_run_script(const char *path) {
    (void)path;
    return TERRA_STATUS_NOSYS;
}
