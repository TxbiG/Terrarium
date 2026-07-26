#include "lifecycle.h"

#include "kernel.h"
#include "arch/arch.h"
#include "drivers/drivers.h"
#include "fs/fs.h"
#include "interrupt/interrupt.h"
#include "ipc/ipc.h"
#include "mm/mm.h"
#include "module/module.h"
#include "net/net.h"
#include "process/process.h"
#include "security/security.h"
#include "syscalls/syscall_table.h"
#include "time/timekeeper.h"

static kernel_shutdown_reason_t last_shutdown_reason = KERNEL_SHUTDOWN_REQUESTED;
static int shutdown_started;

kernel_shutdown_reason_t kernel_last_shutdown_reason(void) {
    return last_shutdown_reason;
}

void kernel_shutdown(kernel_shutdown_reason_t reason) {
    last_shutdown_reason = reason;

    if (shutdown_started) {
        for (;;)
            arch_halt();
    }

    shutdown_started = 1;
    system_state = SYSTEM_SHUTTING_DOWN;

    interrupt_disable();
    syscalls_shutdown();
    ipc_shutdown();
    fs_shutdown();
    net_shutdown();
    module_shutdown();
    drivers_shutdown();
    security_shutdown();
    time_shutdown();
    interrupt_shutdown();
    mm_terminate();

    system_state = SYSTEM_POWER_OFF;
    arch_poweroff();

    for (;;)
        arch_halt();
}

void kernel_reboot(void) {
    last_shutdown_reason = KERNEL_SHUTDOWN_REBOOT;

    if (!shutdown_started) {
        shutdown_started = 1;
        system_state = SYSTEM_RESTART;

        interrupt_disable();
        syscalls_shutdown();
        ipc_shutdown();
        fs_shutdown();
        net_shutdown();
        module_shutdown();
        drivers_shutdown();
        security_shutdown();
        time_shutdown();
        interrupt_shutdown();
        mm_terminate();
    }

    arch_reboot();

    for (;;)
        arch_halt();
}

void kernel_panic(void) {
    last_shutdown_reason = KERNEL_SHUTDOWN_PANIC;
    system_state = SYSTEM_PANIC;

    interrupt_disable();
    syscalls_shutdown();
    net_shutdown();
    drivers_shutdown();
    security_shutdown();

    for (;;)
        arch_halt();
}
