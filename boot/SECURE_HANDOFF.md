# Secure Kernel Handoff

The bootloader should hand control to the TerrariumOS kernel only after building a validated `terra_boot_info_t` structure from `kernel/include/bootloader.h`.

## Handoff Steps

1. Select the boot entry from `boot.cfg`.
2. Load the kernel and optional initrd/modules into memory.
3. Build `terra_boot_info_t` with:
   - architecture and boot protocol
   - kernel physical range
   - memory map
   - framebuffer info
   - ACPI RSDP or device tree pointer
   - command line
   - module list
   - SMP/CPU information when available
4. Validate the structure with `terra_boot_validate_handoff`.
5. Finalize it with `terra_boot_secure_finalize`, which writes the checksum.
6. Disable or exit firmware services that the kernel must not call later.
7. Install final page tables and an isolated kernel stack.
8. Mark bootloader memory as reserved or bootloader-reclaimable in the memory map.
9. Jump to the kernel entry with the handoff pointer in the platform ABI register.

## Architecture ABI

- x86 32-bit: pass the `terra_boot_info_t *` as the first cdecl argument on the stack.
- x86_64 SysV: pass it in `rdi`.
- x86_64 Microsoft ABI/UEFI: pass it in `rcx`, or use a small ABI shim that moves it to the kernel ABI.
- AArch64: pass it in `x0`.
- RISC-V: pass it in `a0`.

## Rights Transfer

After handoff, the kernel owns:

- CPU execution
- interrupt controller setup
- physical memory allocation
- virtual memory mappings
- framebuffer and console ownership
- loaded modules/initrd
- ACPI/device-tree interpretation

The bootloader must not remain callable as a privileged runtime service unless a deliberately designed runtime ABI exists. Firmware runtime services, if kept, must be mapped and mediated by the kernel.

## Security Rules

- Never jump to a kernel image that failed format, size, range, or checksum validation.
- Never trust `boot.cfg` paths or command lines as privileged facts; they are input.
- Never leave writable bootloader stacks, config buffers, or parser scratch memory mapped executable.
- Never mark kernel/module memory as usable RAM.
- Prefer W^X page permissions before jumping: writable data is not executable, executable code is not writable.
- Pass only bounded strings and bounded arrays to the kernel.
- Include a handoff checksum so the kernel can reject corrupted boot info.

## Current Implementation Points

- `kernel/include/bootloader.h`: shared handoff ABI.
- `boot/core/handoff.c`: bootloader-side handoff builder and finalizer.
- `kernel/bootloader.c`: kernel-side validation and import.
- `boot/core/gfx/GUI.c`: graphical multiboot menu surface.
