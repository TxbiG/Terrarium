#ifndef TERRARIUM_BOOT_X86_UEFI_BINDINGS_H
#define TERRARIUM_BOOT_X86_UEFI_BINDINGS_H

/*
 * Bind Terrarium's UEFI protocol table to the firmware-provided EFI image
 * handle and EFI system table. The platform entry code should call this before
 * loader_main() when entered as an EFI application.
 */
int terra_uefi_bind_firmware_services(void *image_handle, void *system_table);

#endif
