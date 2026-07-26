#include "history.h"
#include "../status.h"

int terra_shell_history_add(const char *line) {
    (void)line;
    return TERRA_STATUS_NOSYS;
}

int terra_shell_history_get(unsigned long index, char *out_line, unsigned long out_size) {
    (void)index;
    (void)out_line;
    (void)out_size;
    return TERRA_STATUS_NOSYS;
}

int terra_shell_history_save(const char *path) {
    (void)path;
    return TERRA_STATUS_NOSYS;
}
