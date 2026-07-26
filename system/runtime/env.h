#ifndef TERRARIUM_SYSTEM_RUNTIME_ENV_H
#define TERRARIUM_SYSTEM_RUNTIME_ENV_H

#include <stddef.h>

const char *terra_env_get(const char *name);
int terra_env_set(const char *name, const char *value, int overwrite);
int terra_env_unset(const char *name);
size_t terra_env_export(char *buffer, size_t buffer_size);

#endif
