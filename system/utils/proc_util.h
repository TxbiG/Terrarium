#ifndef TERRARIUM_SYSTEM_UTILS_PROC_UTIL_H
#define TERRARIUM_SYSTEM_UTILS_PROC_UTIL_H

#include "../runtime/process.h"

int terra_proc_find_by_name(const char *name, terra_pid_t *out_pid);
int terra_proc_is_alive(terra_pid_t pid);

#endif
