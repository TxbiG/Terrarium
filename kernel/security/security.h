#ifndef TERRARIUM_SECURITY_H
#define TERRARIUM_SECURITY_H

#include <bootloader.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum terra_security_decision {
    TERRA_SECURITY_DENY = 0,
    TERRA_SECURITY_ALLOW = 1
} terra_security_decision_t;

typedef uint64_t terra_security_caps_t;

#define TERRA_CAP_NONE 0ull
#define TERRA_CAP_SYS_BOOT (1ull << 0)
#define TERRA_CAP_MODULE_LOAD (1ull << 1)
#define TERRA_CAP_MODULE_UNLOAD (1ull << 2)
#define TERRA_CAP_MODULE_EXPORT (1ull << 3)
#define TERRA_CAP_NET_ADMIN (1ull << 4)
#define TERRA_CAP_NET_BIND (1ull << 5)
#define TERRA_CAP_NET_RAW (1ull << 6)
#define TERRA_CAP_FS_ADMIN (1ull << 7)
#define TERRA_CAP_IPC_ADMIN (1ull << 8)
#define TERRA_CAP_TIME_ADMIN (1ull << 9)
#define TERRA_CAP_DEBUG (1ull << 10)
#define TERRA_CAP_ALL (~0ull)

typedef enum terra_security_subject {
    TERRA_SECURITY_SUBJECT_KERNEL = 0,
    TERRA_SECURITY_SUBJECT_MODULE = 1,
    TERRA_SECURITY_SUBJECT_PROCESS = 2,
    TERRA_SECURITY_SUBJECT_DRIVER = 3
} terra_security_subject_t;

typedef enum terra_security_action {
    TERRA_SECURITY_ACTION_SYSCALL = 0,
    TERRA_SECURITY_ACTION_MODULE_LOAD,
    TERRA_SECURITY_ACTION_MODULE_UNLOAD,
    TERRA_SECURITY_ACTION_MODULE_EXPORT,
    TERRA_SECURITY_ACTION_NET_INTERFACE,
    TERRA_SECURITY_ACTION_NET_SOCKET,
    TERRA_SECURITY_ACTION_NET_RAW,
    TERRA_SECURITY_ACTION_IPC,
    TERRA_SECURITY_ACTION_DEBUG
} terra_security_action_t;

typedef struct terra_security_context {
    uint32_t subject_id;
    terra_security_subject_t subject_type;
    terra_security_caps_t caps;
} terra_security_context_t;

typedef struct terra_security_audit_event {
    uint64_t sequence;
    uint32_t subject_id;
    uint32_t action;
    uint32_t object_id;
    terra_security_decision_t decision;
    terra_security_caps_t required;
    terra_security_caps_t present;
} terra_security_audit_event_t;

void security_init(const terra_boot_info_t *boot_info);
int security_is_ready(void);
void security_shutdown(void);
terra_security_decision_t security_check_syscall(uint32_t syscall_id);
terra_security_decision_t security_check(const terra_security_context_t *context,
                                         terra_security_action_t action,
                                         uint32_t object_id,
                                         terra_security_caps_t required_caps);
terra_security_decision_t security_check_caps(terra_security_caps_t present,
                                              terra_security_caps_t required);
void security_set_enforcing(int enabled);
int security_is_enforcing(void);
terra_security_caps_t security_kernel_caps(void);
int security_audit_read(uint32_t index, terra_security_audit_event_t *out_event);
uint32_t security_audit_count(void);

#ifdef __cplusplus
}
#endif

#endif
