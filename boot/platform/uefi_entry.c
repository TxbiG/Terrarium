#include "x86_uefi_bindings.h"

#include <stdint.h>

extern void loader_main(void);

/*
 * EFI application entry point. Firmware calls this with the image handle and
 * system table; we bind Boot Services into Terrarium's platform protocol table
 * before entering the normal bootloader flow.
 */
uint64_t efi_main(void *image_handle, void *system_table) {
    if (terra_uefi_bind_firmware_services(image_handle, system_table) != 0)
        return 1;
    loader_main();
    return 1;
}
