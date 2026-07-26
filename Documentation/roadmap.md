# TerrariumOS Implementation Roadmap

This roadmap tracks the remaining work needed to turn TerrariumOS from a broad
set of subsystem boundaries, hosted bootstrap backends, imported source trees,
and sample applications into a bootable, service-backed desktop operating
system.

The main dependency chain is:

```text
toolchain/build -> boot handoff -> kernel core -> user ABI -> system APIs
-> services -> compositor/windowing -> apps and utilities -> packaging/recovery
-> tests and docs
```

## Current Snapshot

- The repository has clear top-level areas for `boot`, `kernel`, `lib`,
  `services`, `system`, `utilities`, `apps`, `resources`, `smoke`, and
  `Documentation`.
- Build hygiene is in place: apps are gated, the Windows Clang/Ninja presets are
  documented in CMake, and the smoke target exists. Full local verification is
  still blocked when Ninja/compiler tools are not visible in the shell.
- x86_64 emulator boot is the first accepted boot target. Boot configuration,
  secure handoff structures, boot-info validation, staged boot-image scripts,
  and x86_64 boot documentation exist. The shared x86 platform backend models framebuffer, input, preloaded files,
  memory maps, ACPI RSDP, SMP metadata, and final kernel entry. Explicit BIOS/UEFI firmware IO boundaries, BIOS/UEFI protocol tables, UEFI firmware-service bindings, an EFI entry point, an opt-in BIOS real-mode interrupt binding layer, and a boot-image wrapper script now exist. Handoff validation covers ABI identity,
  structure size, kernel ranges, memory-map ranges, module ranges, overlap
  checks, initrd modules, and finalized checksums.
- The kernel has lifecycle boundaries for architecture, memory, interrupts,
  IPC, filesystems, drivers, security, syscalls, time, modules, networking, GPU,
  and devices. Shared status values, normalized VFS errors/open flags,
  `/dev/full`, VM range helpers, timekeeper status returns, filesystem syscall
  IDs, process lifecycle boundaries, driver metadata, GPU ABI scaffolding, and
  network tables are already present.
- The driver registry publishes internal `/dev/<driver>` metadata with resource
  and online-state fields. GPU metadata names render devices, buffers, mmap
  metadata, fences, and command submission. Network metadata covers interfaces,
  routes, DNS servers, firewall rules, socket stats, and packet diagnostics.
- `system` is a compiled `terrarium-system` static library with public status
  values, syscall ABI IDs, hosted syscall stubs, process/service/hardware
  wrappers, network wrappers, shell/filesystem helpers, package/update/signature
  backends, recovery state, capability/audit/sandbox/secrets APIs, crash report
  rings, symbol registration/loading, and deterministic backtrace fallback.
- `services` contains buildable service boundaries plus `terrarium-init`,
  manifest-backed unit metadata, dry-run control commands, login developer
  account data, shell service scaffolding, optional GUI gates, Lumina compositor
  state/event/damage/input lifecycle scaffolding, and Nova protocol-client
  scaffolding.
- `lib` contains Terra UI/system/window APIs, `terrarium-window-client`,
  `terrarium-app-runtime`, an opt-in Wayland/xdg-shell client target,
  lightweight Wayland pieces, imported Mesa/Wayland/wlroots trees, and the
  prebuilt Mesa software-rendering-first import boundary.
- `utilities` builds a broad command set. Networking utilities prefer
  `system/net` wrappers, Phase 9 planned utilities exist, and Phase 10 `pkg`,
  `updatectl`, `recoveryctl`, `auditctl`, `sandboxctl`, and `caps` now use
  system APIs where hosted backends exist.
- `apps` has an app-local CMake boundary, base/experimental gates, an installed
  base-app manifest, and Calculator, Terminal, Task Manager, File Manager,
  Notes, and Settings ported to the Terra app/window lifecycle.
- Phase 10 documentation exists in `Documentation/Packaging-Updates-Recovery.md`.
  Phase 11 documentation now includes testing strategy, package management,
  security model, recovery, and initial ADRs for first boot target,
  package/update model, early security posture, and graphics import boundary.
- `terrarium-contract-smoke` covers the window client, Phase 1 x86 boot handoff,
  Phase 1 BIOS/UEFI firmware protocol tables, and Phase 10 package, update,
  signature, recovery, crash, symbol, and capability contracts.

## Phase 1: Boot Path and Secure Kernel Handoff

Goal: boot one target architecture into the kernel with validated handoff data.

Remaining work:

- Finish firmware validation for x86_64 BIOS/UEFI: complete the BIOS real-mode trampoline and VBE/RSDP export around the new INT 13h/E820/INT 16h binding symbols; validate the new UEFI Simple File System, Block IO, GetMemoryMap, GOP, Simple Text Input, ACPI table, and `ExitBootServices` bindings under OVMF.
- Connect PS/2 or EFI text-input callbacks to boot menu keyboard input.
- Validate the new boot-image wrapper with installed external tools (`mtools` or
  `xorriso`) and confirm the resulting image is firmware-bootable.
- Run the first QEMU boot that reaches a kernel log and record the exact
  command/output.
- Keep ARM, AArch64, and RISC-V firmware paths documented as placeholders until
  they are actively tested.

## Phase 2: Kernel Core Contracts

Goal: expose stable kernel behavior that user space can depend on.

Remaining work:

- Finish scheduler/process/thread semantics: creation, exit status, wait,
  signals, process groups, kernel threads, and user/kernel separation.
- Finish virtual memory: per-process address spaces, user mappings, mmap-style
  regions, shared memory, copy-in/copy-out, guard pages, and page-fault policy.
- Complete syscall dispatch with security policy, ABI versioning, argument
  validation, stable errors, and real handlers behind the hosted system APIs.
- Complete interrupt/APIC/SMP startup and use bootloader CPU-count data instead
  of assuming one CPU.
- Convert x86 exception termination into normal signal/process semantics.
- Finish timekeeping, timers, sleep, monotonic/wall-clock APIs, and timer-driven
  scheduling.

## Phase 3: Filesystems, Devices, and Storage

Goal: support persistent storage and virtual filesystems needed by services and
utilities.

Remaining work:

- Replace remaining fakefs-backed `/dev`, `/proc`, and `/sys` behavior with
  Terrarium-native VFS nodes.
- Finish PTY behavior: EOF/control-D, ENOTTY, buffer limits, SIGHUP,
  controlling terminal behavior, and standard errors.
- Finish VFS mount locking, special-file handling, poll/stat behavior, path
  sanitization, and mount lifecycle.
- Complete ext2 read/write correctness before relying on persistent storage.
- Keep ext4, NTFS, XFS, and exFAT write support disabled until crash-safe
  metadata and journal logic is implemented.
- Connect ATA/NVMe/removable block devices to VFS mount paths.
- Publish hotplugged devices, permissions, and stable metadata through the
  device model.

## Phase 4: Driver and Hardware Model

Goal: make drivers discoverable, attachable, and safe for services to consume.

Remaining work:

- Extend PCI/resource discovery so drivers receive MMIO, IO ports, IRQs, BARs,
  DMA constraints, and identity data.
- Wire NIC probe paths for `e1000`, `ne2k`, `rtl8139`, and `rtl8169` through
  real PCI resources.
- Back `/sys`, `lsdev`, `lspci`, `lsusb`, power services, and hardware inventory
  with kernel device metadata.
- Add driver/module load/unload lifecycle, dependency resolution, permissions,
  signing policy, and failure rollback.
- Implement input drivers and event routing for keyboard, mouse, touch, stylus,
  and controllers.
- Implement audio kernel ABI boundaries for Reson/audio services.
- Add ACPI/uACPI platform callbacks after memory, IO, locks, timers, and
  interrupt abstractions are ready.

## Phase 5: User-Space ABI and System API Library

Goal: turn hosted system boundaries into real kernel/service-backed APIs.

Remaining work:

- Replace hosted syscall stubs with real kernel syscall handlers and service
  endpoints.
- Complete runtime APIs for threads, environment, signal, executable loading,
  shared memory, and dynamic loading policy.
- Complete auth APIs for users, groups, password/shadow validation, PAM-style
  checks, account policy, and privilege transitions.
- Replace hosted filesystem, network, hardware, package, recovery, logging, and
  diagnostics backends with durable kernel/service-backed implementations.
- Expand examples/tests so services, utilities, and apps target stable public
  contracts only.

## Phase 6: Init, Services, and Session Startup

Goal: boot into a service-managed user session.

Remaining work:

- Turn init/service-manager scaffolding into real service execution with
  dependency ordering, restart policy, environment setup, status, and shutdown.
- Replace developer login data with real auth/password/shadow/account policy.
- Finish session startup: `HOME`, `SHELL`, profiles, privilege separation,
  session tokens, and failure handling.
- Turn daemons into long-running processes with event loops, IPC endpoints,
  `/run/...` control sockets, persistent state, and status commands.
- Provide utility endpoints under `/run`, `/proc`, `/sys`, module control paths,
  mount control paths, and service-control paths.
- Implement core services: logging, filesystem/mount, launcher, time, input,
  power, audio, network, DHCP/DNS, firewall, SSH, Bluetooth, update, and
  diagnostics.
- Expand the shell with job control, pipelines, redirects, variables, globbing,
  history, completion, process groups, and signal handling.

## Phase 7: Graphics, Windowing, and Desktop

Goal: support a persistent graphical session with real app windows.

Remaining work:

- Implement the kernel graphics ABI for hardware Mesa: render-node discovery,
  buffer allocation/import/export, mmap, cache rules, command submission,
  fences, synchronization, and permissions.
- Finish Wayland client behavior: registry binding, xdg-shell configure, close,
  ping, resize, fullscreen, maximize, dispatch, and redraw.
- Decide final Wayland/wlroots vendor-versus-Terrarium build boundaries and add
  only the needed platform glue.
- Finish Lumina as a persistent compositor service: client lifecycle, hotplug,
  input routing, scene graph, damage, decorations, and window management.
- Finish Nova as the desktop shell: taskbar, status providers, workspaces,
  launcher, tray, layer-shell integration, and compositor connection.
- Connect Terra UI widgets to real rendering/event backends with layout,
  drawing, focus, accessibility, and input behavior.
- Define screenshot, screen recording, clipboard, drag/drop, accessibility, and
  notification protocols after the compositor loop is stable.

## Phase 8: Networking and Internet Services

Goal: support basic network configuration, diagnostics, and secure remote access.

Remaining work:

- Connect NIC drivers to the kernel networking boundary with real resources.
- Implement real link state, MAC/IP configuration, routing tables, DNS resolver,
  DHCP client/server, firewall rules, socket statistics, and packet diagnostics.
- Back `ip`, `ifconfig`, `ping`, `netstat`, `ss`, `curl`, `wget`, and `ssh`
  with real service endpoints instead of hosted/stub behavior.
- Add TLS/certificate storage and policy before trusting network downloads or
  updates.
- Implement persistent profiles: static IP, DNS, Wi-Fi credentials, and
  per-service network permissions.

## Phase 9: Applications and Utilities

Goal: make shipped apps useful and connect command-line tools to real OS state.

Remaining work:

- Port remaining graphical apps from TODO prototypes to Terra/Wayland/UI:
  Media Player, Paint, Photos, Maps, Health, Mail, Docs, Sheets, Slides, PDF,
  games, and remaining tools.
- Replace app rendering TODOs with compositor surfaces, SHM/EGL buffers,
  input/event dispatch, text rendering, icons, and lifecycle handling.
- Back Task Manager with live `/proc`, scheduler, memory, and service data.
- Back File Manager with VFS mutations, permissions, mounts, icons, thumbnails,
  file operations, and removable-device events.
- Back Terminal with shell service sessions, PTYs, process groups, copy/paste,
  and scrollback rendering.
- Replace utility endpoint stubs as their kernel/service contracts become real.

## Phase 10: Packaging, Updates, Security, and Recovery

Goal: make the OS maintainable after installation.

Remaining work:

- Replace hosted package stores with durable install databases, archive
  unpacking, dependency databases, filesystem mutation, and crash-safe rollback
  storage.
- Add real cryptographic verification, certificate/trust-root storage,
  revocation persistence, secure update channels, and durable secret storage.
- Connect the update daemon to service timers/event loops and real repository
  download/metadata transport.
- Integrate recovery/safe mode with boot-loader target selection, init recovery
  profiles, fsck/repair adapters, durable log export, and service disablement
  persistence.
- Enforce audit, sandbox, capabilities, and privilege policy in the kernel and
  process credential model rather than only hosted user-space stores.
- Add real kernel crash dumps, symbol indexes, and process/kernel backtrace
  capture.

## Phase 11: Documentation and Tests

Goal: keep the project understandable as implementation catches up to intent.

Remaining work:

- Add dedicated API pages for syscalls, IPC, filesystem, memory, networking,
  modules, customization, app development, service control, and package archive
  layout details.
- Add ADRs for kernel/user ABI, VFS model, service manager model, package
  archive layout, and security enforcement stages.
- Add unit tests for boot config, handoff checksum, VFS path handling,
  filesystem validators, text/config parsers, package metadata, and utility
  helpers.
- Add emulator integration tests for boot, init, shell, filesystem mount,
  service status, process lifecycle, and simple GUI startup after x86_64 reaches
  a stable kernel log.
- Add a hardware test matrix after the first emulator target is stable.

## Open Decisions

These items need a project decision before implementation goes too far:

- First persistent root filesystem: ext2 first for simplicity then add FAT32 for
  boot media, or a read-only image plus tmpfs until VFS/storage matures.
- User ABI format: Terrarium-specific executable wrapper `.exe`, ELF-compatible
  ABI, or hybrid loader.
- Service manager model: custom Terrarium units, systemd-like compatibility, or
  a smaller init/service-control format.
- Security enforcement stages: permissive developer mode gates, signed modules
  from the start, or staged enforcement by subsystem.

## Suggested First Milestone

The smallest useful milestone is:

1. Configure and build the tree with a documented toolchain preset.
2. Boot x86_64 in an emulator to a kernel log.
3. Mount native `tmpfs`, `devfs`, and `procfs`.
4. Start init, login, and shell.
5. Run basic utilities against real `/proc`, `/dev`, and service endpoints.
6. Launch a framebuffer or early Wayland test surface.

That milestone creates a working spine for the rest of the OS. After it exists,
graphics, networking, storage, packages, and applications can grow against real
contracts instead of placeholders.










