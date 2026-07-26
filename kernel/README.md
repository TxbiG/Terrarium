# Kernel


# Kernel Work Queue

Priorities are based on boot safety, data integrity, and whether the missing work blocks other subsystems.

## High Priority

- Replace `fs/fakefs` use in `proc`, `dev`, and `sys` with Terrarium-native virtual filesystem nodes. Current files still depend on `fakefs`: `fs/proc.h`, `fs/dev.h`, `fs/proc/proc_controller.c`, `fs/dev/dev_controller.c`, `fs/dev/dev_event.c`, `fs/dev/dev_pty.c`, and `fs/sys_controller.c`.
- Finish PTY/device filesystem behavior in `fs/dev/dev_pty.c`. It is marked as mostly a stub and still needs proper errors, EOF/control-D handling, ENOTTY handling, buffer-limit behavior, and SIGHUP/control-terminal handling when the master closes.
- Finish VFS mount locking and special-file behavior. Open items are in `fs/vfs/vfs_mnt.c`, `fs/vfs/vfs_controller.c`, `fs/vfs/vfs_poll.c`, and `fs/vfs/vfs_stat.c`.
- Complete ext2 read/write correctness before relying on it for persistent storage. Open items include triply indirect reads, singly/doubly/triply indirect writes, indirect-block accounting, indirect-block cleanup, directory-entry removal, deletion semantics, write locking, and one noted memory leak in `fs/ext2/ext2_util.c`, `fs/ext2/ext2_controller.c`, and `fs/ext2/ext2_dirs.c`.
- Extend PCI/resource discovery so hardware drivers can auto-attach safely. The NIC drivers currently register with `nic_controller`, but `e1000`, `ne2k`, `rtl8139`, and `rtl8169` probe paths cannot attach without real MMIO/IO/IRQ resource data.
- Finish SMP/APIC CPU startup. `arch/x86/cpu/apic.c` still loops over one CPU and notes that it should use `bootloader.smp->cpu_count`.

## Medium Priority

- Replace primitive `/dev` setup with a proper device model. `fs/dev/dev_controller.c` is marked primitive and still builds static nodes for stdin, stdout, stderr, tty, fb0, null, random, urandom, ptmx, pts, and input.
- Finish `/proc` layout and process/thread reporting. `fs/proc/proc_controller.c` notes that more entries are needed, `<pid>/task` limits need work, lseek/read pointer behavior needs standardizing, and thread-kill behavior is unfinished.
- Harden `/sys` output around PCI and graphics devices. `fs/sys_controller.c` still exposes fakefs-backed files and should be driven by real kernel device metadata.
- Finish FAT32 directory and filename edge cases. `fs/fat32/fat32_controller.c` notes rough directory handling, and `fs/fat32/fat32_util.c` still needs forbidden-character accounting.
- Replace ad-hoc assert/error paths in VFS and device code with consistent Terrarium error codes. Examples are in `fs/vfs/vfs_controller.c` and `fs/dev/dev_pty.c`.
- Convert x86 exception task termination into Terrarium signal/process semantics. `arch/x86/cpu/isr.c` currently kills the current task with a fixed status where signal handling is expected.

## Low Priority

- Clean up comments and naming left from imported or transitional code once the behavior is replaced, especially `fakefs` field names in `fs/vfs.h` and comments that describe compatibility hacks.
- Optimize ext2 write paths to avoid extra HHDM/memory copies after correctness is finished.
- Replace hard-coded/stat placeholder values, such as the ext2 `st_dev` value in `fs/ext2/ext2_controller.c`.
- Review bundled `external/uACPI` TODO/FIXME items separately from Terrarium-owned kernel work. Those are third-party/vendor tasks unless Terrarium needs to override a platform callback.


# Kernel Subsystem Status

This folder now has a clean boot-time boundary for the kernel subsystems:

- `arch`: selects x86, ARM, or RISC-V initialization from the kernel build options.
- `mm`: starts a boot heap and exposes memory-manager entry points without duplicate header definitions.
- `interrupt`: exposes init, ready-state, enable, and disable entry points.
- `ipc`: exposes init and ready-state entry points.
- `fs`: exposes init and ready-state entry points.
- `drivers`: exposes init and ready-state entry points for bus, storage, input, GPU, audio, and network driver registration.
- `security`: exposes init, ready-state, and syscall policy decision entry points.
- `syscalls`: exposes a bounded syscall table, registration API, and security-mediated dispatch.
- `time`: exposes init, ready-state, monotonic time, and tick update entry points.
- `module`: exposes init, ready-state, and boot-module lookup entry points.
- `net`: exposes init, ready-state, and link-status entry points while lwIP integration remains behind the network boundary.
- `include`: holds the shared boot handoff and kernel-facing headers.
- `external/uACPI`: is available behind `TERRARIUM_USE_UACPI` for ACPI table work once the platform callbacks are implemented.

The imported filesystem, networking, module-loader, driver, and low-level memory sources are intentionally not all linked yet. Many of those files need TerrariumOS-specific VFS, block-device, allocator, locking, scheduler, and architecture glue before they can safely compile as kernel code.
