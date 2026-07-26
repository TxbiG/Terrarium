#ifndef TERRARIUM_LIFECYCLE_H
#define TERRARIUM_LIFECYCLE_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum kernel_shutdown_reason {
    KERNEL_SHUTDOWN_REQUESTED = 0,
    KERNEL_SHUTDOWN_REBOOT = 1,
    KERNEL_SHUTDOWN_PANIC = 2,
} kernel_shutdown_reason_t;

void kernel_shutdown(kernel_shutdown_reason_t reason);
void kernel_reboot(void);
void kernel_panic(void);
kernel_shutdown_reason_t kernel_last_shutdown_reason(void);

#ifdef __cplusplus
}
#endif

#endif
