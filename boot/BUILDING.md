# Building Boot Images

The boot folder now has the structural pieces for:

- config-driven single boot
- graphical multiboot menu
- x86/x86_64, AArch64, and RISC-V handoff ABIs
- platform backend slots
- staged boot-image packaging

## Configure

```powershell
.\Scripts\configure_boot.ps1 -Mode multi -Arch x86_64
```

## Stage Files

```powershell
.\Scripts\package_boot.ps1 -Mode multi -Arch x86_64 -Kernel .\build\kernel\kernel.elf
```

This creates:

```text
build/boot-image/
  boot/
    boot.cfg
    initrd.img              # optional
    x86_64/
      terrarium-kernel.bin
```

## Remaining Hardware Work

The source tree is wired, but the real firmware calls still need target-specific code:

- BIOS INT 13h or disk driver reads
- UEFI file and block IO
- UEFI GOP framebuffer discovery
- BIOS VBE framebuffer discovery
- E820/UEFI/SBI memory map collection
- PS/2, UEFI, serial, or board-specific input
- ExitBootServices on UEFI before kernel entry
