# Boot

TerrariumOS supports two boot modes:

- `single`: boot directly into TerrariumOS.
- `multi`: show a small Terrarium boot menu and boot one of several configured entries.

The multi mode should feel similar to GRUB from a user's point of view, but it is not GRUB syntax and does not depend on GRUB internals. The boot menu reads Terrarium's own simple `key=value` config format.

## Build Defines

```c
#define TERRA_SINGLE_BOOT 1
#define TERRA_MULTI_BOOT 1
```

Use only one mode for a final boot image.

## Architectures

The boot config model currently names these CPU targets:

- `x86`
- `x86_64`
- `arm32`
- `aarch64`
- `riscv32`
- `riscv64`

For RISC-V, use `riscv64` unless you specifically need 32-bit RISC-V. "RISC-V" is the architecture family name; `riscv32` and `riscv64` are the concrete boot targets.

## Config Files

Example configs live in `boot/config/`:

- `single-x86_64.cfg`
- `single-aarch64.cfg`
- `single-riscv64.cfg`
- `multi-x86_64.cfg`
- `multi-aarch64.cfg`
- `multi-riscv64.cfg`

To generate the active boot config from Windows PowerShell:

```powershell
.\Scripts\configure_boot.ps1 -Mode multi -Arch x86_64
```

From a Unix-like shell:

```sh
./Scripts/configure_boot.sh multi x86_64
```

Both commands write `boot/boot.cfg`.

Each config has global keys, followed by one or more `[entry]` blocks:

```ini
mode=multi
timeout_ms=5000
default=0

[entry]
name=TerrariumOS
arch=x86_64
protocol=terrarium
kernel=/boot/x86_64/terrarium-kernel.bin
initrd=/boot/initrd.img
cmdline=root=auto console=tty0
```

## Boot Protocols

- `terrarium`: native TerrariumOS kernel handoff.
- `multiboot1`: legacy x86 Multiboot v1 kernel handoff.
- `multiboot2`: x86 Multiboot v2 kernel handoff.
- `efi-stub`: UEFI firmware loads an EFI-format kernel image, useful for x86_64 and AArch64.
- `devicetree`: load kernel with a device tree pointer, useful for ARM and RISC-V boards.

Important: Multiboot1 and Multiboot2 are primarily x86 boot protocols. The Terrarium multi menu can list ARM and RISC-V entries, but those entries should normally use `efi-stub` or `devicetree`.

## Graphical Menu

The graphical multiboot menu is implemented as a small framebuffer UI in `core/gfx/GUI.c`.
It is intentionally firmware-neutral:

- x86 can provide a VBE/GOP framebuffer.
- AArch64 UEFI can provide a GOP framebuffer.
- RISC-V firmware or a previous boot stage must provide a framebuffer, or the loader should fall back to text/serial.

The menu code needs two platform hooks before it can be shown on real hardware:

- a framebuffer address, width, height, and pitch
- a keyboard/input reader that returns up/down/enter keys

## Platform Defaults

- Desktop: `multi` optional, usually `x86_64`.
- Console: `single`.
- Server: `single` or `multi`, depending on deployment needs.
- Raspberry Pi / ARM board: `single-aarch64.cfg` first, menu later.
- Mobile / Tablet / TV / XR: `single`.

## Files

- `include/boot_config.h`: boot mode, architecture, protocol, and entry structures.
- `include/boot_platform.h`: platform hooks for framebuffer, input, file loading, memory map, and final kernel entry.
- `core/boot_config.c`: parser for the Terrarium boot config format.
- `core/handoff.c`: native Terrarium kernel handoff builder and validator.
- `core/platform_common.c`: shared final kernel-entry call.
- `platform/*`: firmware-specific backends for BIOS, UEFI, AArch64 firmware, and RISC-V SBI.
- `arch/x86/*`: current x86/x86_64 assembly entry stubs.
- `arch/arm/*`: ARM/AArch64 placeholders.
- `arch/riscv/*`: RISC-V placeholders.

See `SECURE_HANDOFF.md` for how the bootloader transfers ownership to the kernel securely.
See `BUILDING.md` for staging boot files into an image directory.
