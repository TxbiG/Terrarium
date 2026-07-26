#include "audit.h"

#include <stdio.h>
#include <string.h>

#include "../status.h"

#define TERRA_AUDIT_RING_MAX 64u

static terra_audit_record_t g_audit[TERRA_AUDIT_RING_MAX];
static unsigned long g_audit_count;
static int g_audit_enabled = 1;

int terra_audit_write(uint32_t subject, uint32_t action, terra_audit_decision_t decision, const char *message) {
    if (!g_audit_enabled)
        return TERRA_STATUS_OK;
    unsigned long slot = g_audit_count % TERRA_AUDIT_RING_MAX;
    g_audit[slot].sequence = g_audit_count + 1;
    g_audit[slot].subject = subject;
    g_audit[slot].action = action;
    g_audit[slot].decision = decision;
    snprintf(g_audit[slot].message, sizeof(g_audit[slot].message), "%s", message ? message : "");
    ++g_audit_count;
    return TERRA_STATUS_OK;
}

int terra_audit_read(unsigned long index, terra_audit_record_t *out_record) {
    if (!out_record)
        return TERRA_STATUS_INVAL;
    unsigned long visible = g_audit_count < TERRA_AUDIT_RING_MAX ? g_audit_count : TERRA_AUDIT_RING_MAX;
    if (index >= visible)
        return TERRA_STATUS_NOENT;
    unsigned long first = g_audit_count > TERRA_AUDIT_RING_MAX ? g_audit_count % TERRA_AUDIT_RING_MAX : 0;
    *out_record = g_audit[(first + index) % TERRA_AUDIT_RING_MAX];
    return TERRA_STATUS_OK;
}

int terra_audit_count(void) {
    return (int)(g_audit_count < TERRA_AUDIT_RING_MAX ? g_audit_count : TERRA_AUDIT_RING_MAX);
}

int terra_audit_set_enabled(int enabled) {
    g_audit_enabled = enabled ? 1 : 0;
    return TERRA_STATUS_OK;
}

int terra_audit_is_enabled(void) {
    return g_audit_enabled;
}
