 <div align="center"><img width=150 height=150 src="https://github.com/TxbiG/TerrariumOS/blob/main/Documentation/images/icons/TerrariumLogo.png"/></div>
 
[![Build](https://github.com/TxbiG/Terrarium/actions/workflows/build.yml/badge.svg)](https://github.com/TxbiG/Terrarium/actions/workflows/build.yml)

# Terrarium OS
**Terrarium OS** is a graphical operating system project focused on simplicity, usability, and a clean foundation for both personal and professional computing.

The project aims to provide a modern operating system environment with a graphical user interface, a modular system architecture, and support for multiple hardware platforms.

> **Status:** Early development

## Overview
Terrarium OS is an experimental operating system designed to explore a complete software stack that includes:

* a boot process and low-level system initialization
* a kernel and core runtime libraries
* user-space services
* graphical system components
* preinstalled applications and utilities
* documentation and development tooling

The long-term goal is to create an operating system that is approachable for everyday use while remaining structured enough for development, experimentation, and platform growth.

## Project Goals

Terrarium OS is being developed around a few core goals:

* **Simplicity** — provide a clean and understandable system structure
* **Usability** — focus on a graphical experience suitable for day-to-day tasks
* **Modularity** — separate the operating system into maintainable components such as boot, kernel, services, applications, and libraries
* **Portability** — target multiple CPU architectures where practical
* **Extensibility** — allow the platform to grow with additional applications, services, and system capabilities over time

## Documentation
Project documentation is available in the [`Documentation/`](./Documentation/README.md) directory.

## System Requirements

The following values represent **target or development requirements** and may change as the operating system matures.

### Minimum development targets

* **CPU:** x86/x64, ARM32/ARM64, or RISC-V
* **Memory:** 4 GB RAM
* **Storage:** 15 GB available disk space
* **Graphics:** Framebuffer-capable graphics adapter
* **System firmware:**
  - x86/x64: UEFI
  - Legacy BIOS
  - ARM32/ARM64: board-specific bootloader / firmware support
  - RISC-V: board-specific firmware support
* **Display:** 1280×720 minimum resolution

As the graphics stack, boot process, and hardware support layers are defined more clearly, these requirements should be updated with concrete platform guidance.



## Repository Structure

```text id="bwy8v0"
.
├─ Documentation/     # Project documentation and design notes
├─ resources/         # Images, audio, and other project resources
├─ apps/              # Applications that may be bundled or preinstalled
├─ boot/              # Bootloader and early boot components
├─ kernel/            # Terrarium OS kernel
├─ lib/               # Shared libraries and core runtime libraries
├─ services/          # User-space operating system services
├─ system/            # System components, configuration, and core assets
└─ utilities/         # Development or system utilities
```


## Development Notes

Terrarium OS is currently a development-stage project, so some areas of the repository may be incomplete or experimental.

Expected areas of ongoing change include:

* build tooling and project structure
* kernel and boot interfaces
* service boundaries and user-space architecture
* graphics and desktop environment design
* hardware support targets
* application and system utility layout

## Contributing
Contributions, suggestions, and design discussions are welcome.

If you want to contribute, a good place to start is:

1. Read the documentation in `Documentation/`
2. Review the current repository structure and component layout
3. Open an issue or discussion for architectural changes or major proposals
4. Keep contributions focused and well-scoped where possible

If the project later adopts contribution guidelines, coding standards, or a formal roadmap, they should be documented in the repository.


## License
Terrarium OS is distributed under the [MIT License](./LICENSE).

## Research
[OS Dev](https://wiki.osdev.org/Expanded_Main_Page), [Operating Systems Course for Beginners](https://youtu.be/yK1uBHPdp30?si=-REyfaX365xTaCre)
- https://wiki.osdev.org/Expanded_Main_Page
- https://github.com/nanobyte-dev/nanobyte_os/tree/master
- https://arjunsreedharan.org/post/99370248137/kernels-201-lets-write-a-kernel-with-keyboard
- https://arjunsreedharan.org/post/82710718100/kernels-101-lets-write-a-kernel
- https://littleosbook.github.io/
- https://github.com/cfenollosa/os-tutorial
- https://www.joe-bergeron.com/posts/Writing%20a%20Tiny%20x86%20Bootloader/
- https://github.com/alpaka-group/alpaka/tree/develop
- https://medium.com/@lduck11007/operating-systems-development-for-dummies-3d4d786e8ac
- https://www.youtube.com/watch?v=vj65eDbdBzs&ab_channel=MapleCircuit
- https://www.youtube.com/playlist?list=PLHh55M_Kq4OApWScZyPl5HhgsTJS9MZ6M
- https://youtu.be/l_hD99zpPBY?si=A2iUxJ5TNzkrPFvz
- https://youtu.be/EBdzWFyKZ0U?si=EV1jAWWFUB-umGuu
- https://youtu.be/EpFUzjYehxs?si=E_uIgJqNv2i4thdh
- https://youtu.be/yK1uBHPdp30?si=dH1qWriwZMt6TWpM
- https://youtu.be/dOiA2nNJpc0?si=RVT2Z8s9jTwWxk_p
- https://youtube.com/playlist?list=PLBlnK6fEyqRiVhbXDGLXDk_OQAeuVcp2O&si=fs_osimueRS4i2kX
- https://youtube.com/playlist?list=PLmxT2pVYo5LB5EzTPZGfFN0c2GDiSXgQe&si=T4N04qISyXYmkoiF
- https://youtube.com/playlist?list=PLT7NbkyNWaqZYHNLtOZ1MNxOt8myP5K0p&si=BrqXaOLGQT2W-YpG
- Explaining File Systems: NTFS, exFAT, FAT32, ext4 & More https://www.youtube.com/watch?v=_h30HBYxtws
