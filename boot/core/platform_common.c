#include "../include/boot_platform.h"

#if defined(TERRA_BOOT_X86_PLATFORM)
#include "../platform/x86_platform.h"
#include "../platform/x86_firmware_io.h"
#else
extern void terra_arch_enter_kernel(uint64_t entry, const terra_boot_info_t *handoff);
#endif

void terra_platform_enter_kernel(uint64_t entry, const terra_boot_info_t *handoff) {
#if defined(TERRA_BOOT_X86_PLATFORM)
#if defined(TERRA_BOOT_UEFI_PLATFORM)
    terra_uefi_exit_boot_services();
#endif
    terra_x86_platform_enter_kernel(entry, handoff);
#else
    terra_arch_enter_kernel(entry, handoff);
    for (;;)
        ;
#endif
}



