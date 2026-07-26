#include "secrets.h"

#include <stdio.h>
#include <string.h>

#include "../status.h"

#define TERRA_SECRET_MAX 32u

static terra_secret_record_t g_secrets[TERRA_SECRET_MAX];
static unsigned g_secret_count;

static int find_secret(const char *name) {
    if (!name)
        return -1;
    for (unsigned i = 0; i < g_secret_count; ++i) {
        if (strcmp(g_secrets[i].name, name) == 0)
            return (int)i;
    }
    return -1;
}

int terra_secret_put(const char *name, const char *value) {
    if (!name || !name[0] || !value)
        return TERRA_STATUS_INVAL;
    int index = find_secret(name);
    if (index < 0) {
        if (g_secret_count >= TERRA_SECRET_MAX)
            return TERRA_STATUS_NOSPC;
        index = (int)g_secret_count++;
    }
    snprintf(g_secrets[index].name, sizeof(g_secrets[index].name), "%s", name);
    snprintf(g_secrets[index].value, sizeof(g_secrets[index].value), "%s", value);
    return TERRA_STATUS_OK;
}

int terra_secret_get(const char *name, char *out_value, unsigned long out_size) {
    if (!name || !name[0] || !out_value || out_size == 0)
        return TERRA_STATUS_INVAL;
    int index = find_secret(name);
    if (index < 0)
        return TERRA_STATUS_NOENT;
    snprintf(out_value, out_size, "%s", g_secrets[index].value);
    return TERRA_STATUS_OK;
}

int terra_secret_remove(const char *name) {
    if (!name || !name[0])
        return TERRA_STATUS_INVAL;
    int index = find_secret(name);
    if (index < 0)
        return TERRA_STATUS_NOENT;
    for (unsigned i = (unsigned)index + 1; i < g_secret_count; ++i)
        g_secrets[i - 1] = g_secrets[i];
    --g_secret_count;
    return TERRA_STATUS_OK;
}

int terra_secret_count(void) {
    return (int)g_secret_count;
}
