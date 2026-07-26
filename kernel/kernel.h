#ifndef TERRARIUM_KERNEL_H
#define TERRARIUM_KERNEL_H

#include <bootloader.h>

#include "lifecycle.h"

// compiles into terrarium-kernel.bin

extern enum system_states {
	SYSTEM_BOOTING,
	SYSTEM_SCHEDULING,
	SYSTEM_FREEING_INITMEM,
	SYSTEM_RUNNING,
	SYSTEM_PANIC,
	SYSTEM_SHUTTING_DOWN,
	SYSTEM_HALT,
	SYSTEM_POWER_OFF,
	SYSTEM_RESTART,
	SYSTEM_SUSPEND,
} system_state;



void kmain(const terra_boot_info_t *boot_info);


#endif // TERRARIUM_KERNEL_H
