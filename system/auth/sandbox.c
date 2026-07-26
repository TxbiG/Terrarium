#include "sandbox.h"

#include <string.h>

#include "../status.h"

#define TERRA_SANDBOX_MAX 32u

typedef struct sandbox_slot {
    uint32_t subject;
    terra_sandbox_policy_t policy;
    int active;
} sandbox_slot_t;

static sandbox_slot_t g_sandboxes[TERRA_SANDBOX_MAX];

static int find_slot(uint32_t subject) {
    for (unsigned i = 0; i < TERRA_SANDBOX_MAX; ++i) {
        if (g_sandboxes[i].active && g_sandboxes[i].subject == subject)
            return (int)i;
    }
    return -1;
}

int terra_sandbox_apply(uint32_t subject, const terra_sandbox_policy_t *policy) {
    if (!subject || !policy || !policy->name[0])
        return TERRA_STATUS_INVAL;
    int slot = find_slot(subject);
    if (slot < 0) {
        for (unsigned i = 0; i < TERRA_SANDBOX_MAX; ++i) {
            if (!g_sandboxes[i].active) {
                slot = (int)i;
                break;
            }
        }
    }
    if (slot < 0)
        return TERRA_STATUS_NOSPC;
    g_sandboxes[slot].subject = subject;
    g_sandboxes[slot].policy = *policy;
    g_sandboxes[slot].active = 1;
    return TERRA_STATUS_OK;
}

int terra_sandbox_release(uint32_t subject) {
    if (!subject)
        return TERRA_STATUS_INVAL;
    int slot = find_slot(subject);
    if (slot < 0)
        return TERRA_STATUS_NOENT;
    memset(&g_sandboxes[slot], 0, sizeof(g_sandboxes[slot]));
    return TERRA_STATUS_OK;
}

int terra_sandbox_get(uint32_t subject, terra_sandbox_policy_t *out_policy) {
    if (!subject || !out_policy)
        return TERRA_STATUS_INVAL;
    int slot = find_slot(subject);
    if (slot < 0)
        return TERRA_STATUS_NOENT;
    *out_policy = g_sandboxes[slot].policy;
    return TERRA_STATUS_OK;
}
