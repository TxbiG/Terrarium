# Utilities
This document lists recommended userland and system utilities for a hybrid modular kernel operating system. The list is organized by category and priority, serving as both a roadmap and a reference.

## Current command set
The `CMakeLists.txt` in this folder now builds every utility source directly. Commands that can operate with standard files, `/proc`, `/sys`, `/run`, or the current environment do so. Commands that require a kernel/service backend fail clearly with the exact missing Terrarium endpoint instead of using a generic fallback.

- `cat`, `clear`, `echo`, `hexdump`
- `less`, `more`
- `cp`, `df`, `du`, `ls`, `mkdir`, `mv`, `pwd`, `rm`, `rmdir`, `stat`
- `env`, `which`
- `chmod`, `chown`, `id`, `whoami`
- `cut`, `diff`, `find`, `grep`, `head`, `sort`, `tail`, `uniq`, `wc`
- `free`, `iostat`, `lsdev`, `lspci`, `lsusb`, `vmstat`, `dmesg`
- `kill`, `ps`, `reboot`, `shutdown`, `suspend`, `top`, `uptime`
- `klog`, `kpanic`, `ksysctl`, `ksysinfo`, `ktrace`
- `ipcs`, `ipcrm`, `ipcstat`, `msgsend`, `rpcinfo`
- `moddeps`, `modinfo`, `modlist`, `modload`, `modprobe`, `modunload`
- `curl`, `ifconfig`, `ip`, `netstat`, `ping`, `ss`, `ssh`, `wget`
- `alias`, `export`, `history`, `set`
- `fsck`, `mount`, `umount`
- `login`, `logout`, `passwd`, `su`, `sudo`
- `nm`, `objdump`, `pstree`, `strace`
- `service`, `sysctl`, `terrarium`, `time`, `uname`

There is no longer a fallback roadmap target path.

## 1. Core Kernel & Module Management
### Module Control
- `modload` (kernel-facing) - Load a kernel module
- `modunload` - Unload a kernel module
- `modlist` - List loaded modules
- `modinfo` - Display module metadata
- `moddeps` - Show module dependency graph
- `modprobe` - Load module with dependencies

### Kernel Introspection
- `klog` - View kernel log buffer
- `ksysinfo` - Kernel version and build info
- `ksysctl` - Runtime kernel parameter control
- `ktrace` - Kernel event tracing
- `kpanic` - Kernel panic dump viewer

## 2. Process & Scheduler Utilities
- `ps` - List processes
- `top` / `htop` - Real-time process monitor
- `proctree` - Process hierarchy view
- `kill` - Send signals to processes
- `nice / renice` - Adjust scheduling priority
- `uptime` - System running time
- `schedstat` - Scheduler statistics
- `shutdown` / `reboot` / `suspend` - power control

## 3. IPC & Messaging
- `ipcs` - List IPC objects
- `ipcrm` - Remove IPC objects
- `ipcstat` - IPC performance statistics
- `msgsend` - Send IPC test messages
- `msgrecv` - Receive IPC test messages
- `rpcinfo` - Registered kernel/user services

## 4. Filesystem & Storage Utilities
### Filesystem Control
- `ls` (userland) - list directory contents
- `cp` / `mv` / `rm` - copy, move, delete files
- `mkdir` / `rmdir` - create/remove directories
- `mount` - Mount filesystems
- `umount` - Unmount filesystems
- `lsfs` - List filesystem drivers
- `fsload` - Load filesystem module
- `fsck` - Filesystem consistency check

### Disk Usage
- `df` - Disk free space
- `du` - Directory disk usage
- `stat` - File metadata

## 5. Device & Driver Management
- `lsdev` - List detected devices
- `drvbind` - Bind driver to device
- `drvunbind` - Unbind driver from device
- `drvinfo` - Driver metadata
- `devstat` - Device I/O statistics
- `hotplug` - Dynamic device handler

## 6. Memory & Virtual Memory
- `free` - Memory usage summary
- `vmstat` - Virtual memory statistics
- `slabtop` - Kernel slab allocator stats
- `pmap` - Process memory maps
- `kmemstat` - Kernel memory usage
- `swapctl` - Swap management

## 7. Service & System Management
- `init` - System initialization
- `initctl` - Init/system manager control
- `servicectl` - Start/stop services
- `svclist` - List services
- `svcinfo` - Service details
- `watchdogctl` - System watchdog control
- `sysctl` - Runtime unified kernel + service parameter control
- `sysinfo` - Provides a detailed summary of your OS, hardware (RAM, BIOS version, network cards) etc.

## 8. User & Security Utilities
- `login` - User login
- `logout` - User logout
- `passwd` - Password management
- `whoami` - Current user
- `id` - User/group identifiers
- `chmod` - Change permissions
- `chown` - Change ownership
- `su` / `sudo` - privilege escalation
- `caps` - Capability inspection
- `sandboxctl` - Process isolation
- `auditctl` - Security auditing control
- `secstat` - Security subsystem status
- `keyctl` - Kernel keyring manager

## 9. Networking Utilities
- `ifconfig / ip` - Network interface configuration
- `ping` - Network reachability
- `route` - Routing table control
- `netstat / ss` - Network socket statistics
- `wget / curl` - Network downloads
- `ssh` - Secure remote shell

## 10. Text & Shell Utilities
### Shell
- `sh` - System shell
- `env` - Environment variables
- `which` - Locate executables
- `history` - Command history
- `alias` - Command aliases
- `export` / `set` - shell variable management

### Text Processing
- `cat` - Display file contents
- `echo` - output text
- `less` / `more` - paginated file viewing
- `grep` - Pattern matching
- `find` - File search
- `sort` - Sort input
- `uniq` - Filter duplicates
- `wc` - Word/line count
- `cut` - Column extraction
- `head / tail` - Partial file view
- `diff` - File comparison

## 11. Developer & Debugging Tools
- `strace` - System call tracing
- `ftrace` - Function tracing
- `perf` - Performance profiling
- `time` - Execution timing
- `hexdump` - Binary inspection
- `objdump` - Object file analysis
- `nm` - Symbol table viewer
- `addr2line` - Address-to-source mapping
- `crash` - Kernel crash analyzer

## Phase 9 utility endpoint policy

The planned utilities `route`, `caps`, `auditctl`, `sandboxctl`, `swapctl`,
`pmap`, `slabtop`, `drvbind`, `drvunbind`, `drvinfo`, `hotplug`, `initctl`,
`servicectl`, and `diagnostics` are now present as command targets. They should
be connected to real system APIs as those APIs land; until then they report the
specific missing `/proc`, `/sys`, `/run`, kernel, or service endpoint.
