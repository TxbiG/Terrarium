#ifndef LIBSYSTEM_H
#define LIBSYSTEM_H

#ifdef __cplusplus
extern "C" {
#endif

/* System power actions */
typedef enum {
    SYSTEM_ACTION_NONE = 0,
    SYSTEM_ACTION_REBOOT,
    SYSTEM_ACTION_SHUTDOWN,
    SYSTEM_ACTION_SUSPEND,
    SYSTEM_ACTION_HIBERNATE
} system_action_t;

/* System lifecycle control */
int system_init(void);
int system_shutdown(void);

/* Power state transitions */
int system_reboot(void);
int system_poweroff(void);
int system_suspend(void);
int system_hibernate(void);

/* Query support */
int system_can_suspend(void);
int system_can_hibernate(void);

#ifdef __cplusplus
}
#endif

#endif /* LIBSYSTEM_H */
