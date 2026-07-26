# System

The `system` folder is the public user-space system API layer for TerrariumOS.
It sits above `kernel`, `lib`, and `services`: applications and service
daemons should include these headers instead of reaching directly into kernel
or service internals.

## Current Build Target

- `terrarium-system`: static library that exposes system headers and hosted boundary implementations.

## API Groups

- `runtime`: process, thread, environment, signal, executable, and syscall APIs.
- `shell`: shell helpers, builtins, completion, history, and job control APIs.
- `utils`: reusable command-line utility helpers.
- `fs`: mount, block device, VFS, UUID, quota, mkfs, and fsck APIs.
- `net`: IP, DNS, route, firewall, ping, and socket helper APIs.
- `auth`: user, group, password, shadow, PAM, and capability APIs.
- `package`: package install, repository, update, and signature APIs.
- `log`: system log, journal, crash, trace, and diagnostic APIs.
- `hardware`: CPU, PCI, battery, thermal, power, and hardware inventory APIs.
- `boot`: boot configuration, initramfs, loader, command line, and sysctl APIs.
- `dev`: debugging, symbols, profiler, sanitizer, and backtrace APIs.
- `text`: config, INI, JSON, YAML, regex, and UTF-8 APIs.
- `time`: clocks, sleep, calendar, timezone, locale, and time APIs.

Most headers are intentionally skeletal for now. They mark stable ownership
boundaries so implementations can be added without mixing service code,
application code, and kernel-facing ABI code.

## System API Work Queue

Priorities focus on turning the public user-space API layer from header
boundaries into usable implementations for utilities, services, and apps.

### High Priority

- Continue filling in real kernel/service-backed implementations behind `terrarium-system`; the current library provides hosted boundary stubs and argument validation for many APIs.
- Implement `runtime` APIs for process, thread, environment, signal,
  executable loading, init, and syscall wrappers. These are needed by shell,
  services, utilities, and app launch.
- Implement `auth` APIs for users, groups, password/shadow records, PAM-style
  checks, and capabilities. These are required by login, sudo/su/passwd, and
  service privilege separation.
- Implement `fs` APIs for VFS, block devices, mounts, UUID, quota, mkfs, and
  fsck. `fs/fsck.h` still exposes `TERRA_FSCK_UNSUPPORTED`, so real filesystem
  check/repair adapters are missing.
- Implement `net` APIs for interfaces, IP addresses, routes, DNS, firewall,
  ping, and socket helpers. These should back the network service and utilities
  like `ip`, `ifconfig`, `ping`, `netstat`, and `ss`.
- Implement `log` APIs for system log, journal, crash reports, trace data, and
  diagnostics. These are needed by `dmesg`, `klog`, `kpanic`, `ktrace`, and
  service debugging.

### Medium Priority

- Implement `hardware` APIs for CPU, PCI, battery, thermal, power, and hardware
  inventory. These should connect to kernel driver/device metadata and power
  services.
- Implement `boot` APIs for boot config, command line, loader state, initramfs,
  and sysctl access.
- Implement `shell` APIs for builtins, completion, history, readline, job
  control, and shell integration.
- Replace the Phase 10 package/update/signature boundary stubs with a real install database, repository metadata store, dependency solver, trusted-root database, cryptographic verifier, and rollback storage.
- Implement `dev` APIs for debug output, symbols, profiler, sanitizer, and
  backtrace support.

### Low Priority

- Implement `text` helpers for config, INI, JSON, YAML, regex, and UTF-8 once
  the service/runtime APIs are stable.
- Implement `time` helpers for clocks, sleep, calendar, timezone, locale, and
  wall-clock conversions over the kernel time and time service APIs.
- Implement reusable utility helpers under `utils` after the concrete command
  behavior in `utilities` has settled.
- Add examples/tests for each public API group so services and apps have a
  stable contract to target.

