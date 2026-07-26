#include "security.h"

#define TERRA_SECURITY_AUDIT_SIZE 128u

static terra_security_audit_event_t audit_log[TERRA_SECURITY_AUDIT_SIZE];
static uint64_t audit_sequence;
static uint32_t audit_head;
static uint32_t audit_count_value;
static int security_ready;
static int security_enforcing;

static terra_security_caps_t action_caps(terra_security_action_t action) {
    switch (action) {
    case TERRA_SECURITY_ACTION_MODULE_LOAD:
        return TERRA_CAP_MODULE_LOAD;
    case TERRA_SECURITY_ACTION_MODULE_UNLOAD:
        return TERRA_CAP_MODULE_UNLOAD;
    case TERRA_SECURITY_ACTION_MODULE_EXPORT:
        return TERRA_CAP_MODULE_EXPORT;
    case TERRA_SECURITY_ACTION_NET_INTERFACE:
        return TERRA_CAP_NET_ADMIN;
    case TERRA_SECURITY_ACTION_NET_SOCKET:
        return TERRA_CAP_NET_BIND;
    case TERRA_SECURITY_ACTION_NET_RAW:
        return TERRA_CAP_NET_RAW;
    case TERRA_SECURITY_ACTION_IPC:
        return TERRA_CAP_IPC_ADMIN;
    case TERRA_SECURITY_ACTION_DEBUG:
        return TERRA_CAP_DEBUG;
    case TERRA_SECURITY_ACTION_SYSCALL:
    default:
        return TERRA_CAP_NONE;
    }
}

static void audit(uint32_t subject_id,
                  terra_security_action_t action,
                  uint32_t object_id,
                  terra_security_decision_t decision,
                  terra_security_caps_t required,
                  terra_security_caps_t present) {
    terra_security_audit_event_t *event = &audit_log[audit_head];
    event->sequence = ++audit_sequence;
    event->subject_id = subject_id;
    event->action = (uint32_t)action;
    event->object_id = object_id;
    event->decision = decision;
    event->required = required;
    event->present = present;

    audit_head = (audit_head + 1u) % TERRA_SECURITY_AUDIT_SIZE;
    if (audit_count_value < TERRA_SECURITY_AUDIT_SIZE)
        audit_count_value++;
}

void security_init(const terra_boot_info_t *boot_info) {
    (void)boot_info;
    for (uint32_t i = 0; i < TERRA_SECURITY_AUDIT_SIZE; ++i) {
        audit_log[i].sequence = 0;
        audit_log[i].subject_id = 0;
        audit_log[i].action = 0;
        audit_log[i].object_id = 0;
        audit_log[i].decision = TERRA_SECURITY_DENY;
        audit_log[i].required = 0;
        audit_log[i].present = 0;
    }
    audit_sequence = 0;
    audit_head = 0;
    audit_count_value = 0;
    security_enforcing = 1;
    security_ready = 1;
}

int security_is_ready(void) {
    return security_ready;
}

void security_shutdown(void) {
    security_ready = 0;
}

terra_security_decision_t security_check_syscall(uint32_t syscall_id) {
    terra_security_context_t kernel = {
        .subject_id = 0,
        .subject_type = TERRA_SECURITY_SUBJECT_KERNEL,
        .caps = TERRA_CAP_ALL,
    };
    return security_check(&kernel,
                          TERRA_SECURITY_ACTION_SYSCALL,
                          syscall_id,
                          TERRA_CAP_NONE);
}

terra_security_decision_t security_check(const terra_security_context_t *context,
                                         terra_security_action_t action,
                                         uint32_t object_id,
                                         terra_security_caps_t required_caps) {
    terra_security_caps_t present = context ? context->caps : TERRA_CAP_NONE;
    uint32_t subject = context ? context->subject_id : 0xffffffffu;
    terra_security_caps_t required = required_caps | action_caps(action);
    terra_security_decision_t decision;

    if (!security_ready) {
        decision = TERRA_SECURITY_DENY;
    } else if (!security_enforcing) {
        decision = TERRA_SECURITY_ALLOW;
    } else {
        decision = security_check_caps(present, required);
    }

    audit(subject, action, object_id, decision, required, present);
    return decision;
}

terra_security_decision_t security_check_caps(terra_security_caps_t present,
                                              terra_security_caps_t required) {
    return (present & required) == required ? TERRA_SECURITY_ALLOW
                                            : TERRA_SECURITY_DENY;
}

void security_set_enforcing(int enabled) {
    security_enforcing = enabled ? 1 : 0;
}

int security_is_enforcing(void) {
    return security_enforcing;
}

terra_security_caps_t security_kernel_caps(void) {
    return TERRA_CAP_ALL;
}

int security_audit_read(uint32_t index, terra_security_audit_event_t *out_event) {
    if (!out_event || index >= audit_count_value)
        return -1;

    uint32_t first = (audit_head + TERRA_SECURITY_AUDIT_SIZE - audit_count_value) %
                     TERRA_SECURITY_AUDIT_SIZE;
    uint32_t slot = (first + index) % TERRA_SECURITY_AUDIT_SIZE;
    *out_event = audit_log[slot];
    return 0;
}

uint32_t security_audit_count(void) {
    return audit_count_value;
}
