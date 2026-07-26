#ifndef TERRARIUM_SYSTEM_AUTH_SANDBOX_H
#define TERRARIUM_SYSTEM_AUTH_SANDBOX_H

#include <stdint.h>

#define TERRA_SANDBOX_NAME_MAX 64u

typedef struct terra_sandbox_policy {
    char name[TERRA_SANDBOX_NAME_MAX];
    uint64_t allowed_caps;
    uint32_t allow_network;
    uint32_t allow_filesystem_write;
    uint32_t allow_debug;
} terra_sandbox_policy_t;

int terra_sandbox_apply(uint32_t subject, const terra_sandbox_policy_t *policy);
int terra_sandbox_release(uint32_t subject);
int terra_sandbox_get(uint32_t subject, terra_sandbox_policy_t *out_policy);

#endif
