# Boot Architecture Status

TerrariumOS can describe boot entries for x86, ARM, and RISC-V CPUs through `boot.cfg`.
Actual booting still depends on an architecture entry stub and firmware path.

## x86 / x86_64

Status: structurally implemented for x86_64 handoff; firmware calls and first QEMU boot still need hardware/emulator testing.

- BIOS boot sector stubs exist in `x86/32/boot.asm` and `x86/64/boot.asm`.
- Multiboot header stubs exist in `x86/32/multiboot.asm` and `x86/64/multiboot.asm`.
- BIOS and UEFI platform sources now delegate through the shared `boot/platform/x86_platform.*` state layer for framebuffer, memory map, input, file loading, ACPI RSDP, SMP metadata, and final kernel entry. Real firmware calls still need to fill that state.

## ARM / AArch64

Status: handoff ABI and backend slot added, board-specific firmware work still needed.

- Boot configs exist for `aarch64`.
- Preferred boot path is UEFI `efi-stub` on systems that provide UEFI.
- Board firmware paths such as Raspberry Pi need a board-specific entry stub and device-tree handoff.

## RISC-V

Status: handoff ABI and backend slot added, SBI/firmware work still needed.

- Boot configs exist for `riscv64`.
- Preferred boot path is firmware/SBI handoff with device tree.
- `riscv32` is named in the config model but does not have a sample config yet.

## Multiboot

Multiboot1 and Multiboot2 are x86-oriented protocols. For ARM and RISC-V, the Terrarium menu can still provide multiple entries, but those entries should use `efi-stub` or `devicetree` handoff rather than Multiboot.

