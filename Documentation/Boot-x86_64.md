# x86_64 Boot Target

The first TerrariumOS boot target is x86_64 in an emulator. This target is the smallest practical Phase 1 path because the repository has x86_64 boot config files, x86 boot entry stubs, a native Terrarium handoff structure, and BIOS/UEFI backend slots backed by a shared x86 platform state model.

## Target

- Architecture: `x86_64`
- Default config: `boot/config/single-x86_64.cfg`
- Kernel load address: `0x00100000`
- Optional initrd load address: `0x04000000`
- Handoff ABI: `terra_boot_info_t *` passed through the x86_64 entry ABI

## Staging

After the kernel target exists in the configured build directory, stage boot files with:

```powershell
.\Scripts\package_x86_64_emulator.ps1 -Kernel build\windows-clang-ninja\kernel\kernel.elf
```

The script wraps `Scripts/package_boot.ps1` and stages:

```text
build/boot-image-x86_64/
  boot/
    boot.cfg
    x86_64/
      terrarium-kernel.bin
```

Add `-Initrd path\to\initrd.img` when an initrd is available. The bootloader now carries a loaded initrd into `terra_boot_info_t.modules[0]` as `initrd`.

## Emulator Command

After staging, create a boot image wrapper. The wrapper currently supports `raw-fat` through mtools and `iso` through xorriso when those external tools are installed:

```powershell
.\Scripts\wrap_x86_64_boot_image.ps1 -Format raw-fat -StagedRoot build\boot-image-x86_64 -Output build\terrarium-x86_64.img
``` 

Use `-DryRun` to print the exact external commands without creating an image. Once the staged boot files are wrapped into a raw bootable disk image, the expected emulator command is:

```powershell
.\Scripts\run_x86_64_qemu.ps1 -DiskImage build\terrarium-x86_64.img
```

The script prints and runs this QEMU invocation:

```text
qemu-system-x86_64 -m 512M -serial stdio -no-reboot -no-shutdown -drive format=raw,file="build\terrarium-x86_64.img"
```

Use `-DryRun` to print the command without launching QEMU. The script intentionally fails if QEMU or the disk image is missing; Phase 1 is not complete until the image wrapper and firmware file-loading path exist.

## Platform Handoff Hooks

The x86/x86_64 boot backend now exposes a shared platform state layer in `boot/platform/x86_platform.*`, an explicit firmware IO boundary in `boot/platform/x86_firmware_io.*`, BIOS protocol tables in `boot/platform/x86_bios_protocol.*`, and UEFI protocol tables in `boot/platform/x86_uefi_protocol.*` for:

- framebuffer discovery results
- keyboard/menu input callbacks
- preloaded file registration and file-copy loading
- memory-map collection results
- ACPI RSDP handoff
- SMP CPU-count/BSP metadata
- final kernel entry callback

BIOS and UEFI backends delegate through these protocol tables. The C-side contracts for INT 13h packet reads, E820 entries, VBE mode info, PS/2 keys, UEFI file/block IO, memory descriptors, GOP mode info, EFI text input, ACPI RSDP, and `ExitBootServices` now exist. UEFI now has a real `efi_main(image_handle, system_table)` service binding path for Simple File System, Block IO, GetMemoryMap, GOP, Simple Text Input, ACPI configuration tables, and ExitBootServices. BIOS now has an opt-in real-mode interrupt binding layer for INT 13h, INT 15h/E820, INT 16h keyboard input, and future VBE/RSDP thunk completion; it still requires the boot-sector real-mode trampoline to pass low-memory buffers and boot-drive state safely.

## Handoff Validation

The bootloader now validates:

- magic and ABI version
- structure size
- kernel physical range
- non-empty bounded memory map
- bounded module count
- non-overflowing memory-map ranges
- non-overflowing module ranges
- modules that do not overlap the kernel image or each other
- finalized checksum before entering the kernel

The kernel independently validates the finalized checksum, kernel range, memory-map ranges, and module ranges before importing the handoff.

## Remaining Hardware Work

This is not a successful boot yet. The remaining Phase 1 hardware/emulator work is:

- complete the BIOS real-mode trampoline around the new INT 13h/INT 15h/INT 16h binding symbols and preserve the boot-drive number
- validate UEFI GetMemoryMap descriptor sizing and map-key reuse immediately before ExitBootServices
- finish BIOS VBE mode-info thunk export; UEFI GOP discovery binding is present
- finish BIOS RSDP scanning/export from EBDA/BIOS ROM; UEFI ACPI table binding is present
- boot the EFI image under OVMF and record the first successful kernel log
- external image-wrapper tools installed and validated (`mtools` for raw FAT or `xorriso` for ISO)
- first successful QEMU boot log once the image wrapper and firmware IO path exist





