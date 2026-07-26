#ifndef TERRARIUM_BOOT_X86_BIOS_REALMODE_H
#define TERRARIUM_BOOT_X86_BIOS_REALMODE_H

#include "x86_bios_protocol.h"

/*
 * Installs BIOS protocol callbacks backed by 16-bit real-mode interrupt
 * shims. These callbacks are only valid while the boot path still has a real
 * mode thunk available; protected/long mode callers must transition through
 * the thunk before invoking BIOS interrupts.
 */
void terra_bios_install_real_mode_bindings(void);

#endif
