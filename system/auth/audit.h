#ifndef TERRARIUM_SYSTEM_AUTH_AUDIT_H
#define TERRARIUM_SYSTEM_AUTH_AUDIT_H

#include <stdint.h>

#define TERRA_AUDIT_MESSAGE_MAX 160u

typedef enum terra_audit_decision {
    TERRA_AUDIT_DENY = 0,
    TERRA_AUDIT_ALLOW = 1
} terra_audit_decision_t;

typedef struct terra_audit_record {
    uint64_t sequence;
    uint32_t subject;
    uint32_t action;
    terra_audit_decision_t decision;
    char message[TERRA_AUDIT_MESSAGE_MAX];
} terra_audit_record_t;

int terra_audit_write(uint32_t subject, uint32_t action, terra_audit_decision_t decision, const char *message);
int terra_audit_read(unsigned long index, terra_audit_record_t *out_record);
int terra_audit_count(void);
int terra_audit_set_enabled(int enabled);
int terra_audit_is_enabled(void);

#endif
