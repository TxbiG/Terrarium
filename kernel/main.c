#include "kernel.h"

#include <terra_acpi.h>
#include "arch/arch.h"
#include "interrupt/interrupt.h"
#include "ipc/ipc.h"
#include "fs/fs.h"
#include "drivers/drivers.h"
#include "security/security.h"
#include "syscalls/syscall_table.h"
#include "time/timekeeper.h"
#include "process/process.h"
#include "module/module.h"
#include "net/net.h"
#include "mm/mm.h"

enum system_states system_state = SYSTEM_BOOTING;

void kmain(const terra_boot_info_t *boot_info) {
    bootloader_import(boot_info);

    arch_early_init(&bootloader);
    mm_init(&bootloader);
    terra_acpi_init(&bootloader);
    interrupt_init(&bootloader);
    time_init(&bootloader);
    drivers_init(&bootloader);
    security_init(&bootloader);
    syscalls_init(&bootloader);
    module_init(&bootloader);
    net_init(&bootloader);
    ipc_init(&bootloader);
    fs_init(&bootloader);
    arch_late_init(&bootloader);

    system_state = SYSTEM_RUNNING;

    for (;;)
        arch_halt();
}
