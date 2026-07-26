# Services

TerrariumOS services are user-space daemons and session programs that sit
above the kernel and libraries. They should be started by the init/service
manager once the kernel has mounted filesystems, started IPC, and exposed the
required device interfaces.

## Current Build Targets

- `terrarium-init`: initial service-manager boundary with manifest-backed `services/init/units/*.service` metadata files and compiled fallback units, dependency ordering, dry-run startup/control commands, and status output.

- `terrarium-shell`: interactive command shell.
- `terrarium-login`: simple text login/session launcher.
- `terrarium-audiod`, `terrarium-fsd`, `terrarium-inputd`,
  `terrarium-launcherd`, `terrarium-logd`, `terrarium-netd`,
  `terrarium-dhcpd`, `terrarium-timed`: small service boundary daemons
  with status entry points.
- `terrarium-nova`: optional desktop shell/status/workspace boundary built
  when `TERRARIUM_BUILD_NOVA` is enabled.
- `terrarium-service-placeholders`: interface target for service headers that
  are shared with utilities and future service clients.

## Planned Service Groups

- Core: IPC bus, cron/at scheduling, logging, privilege policy, user accounts.
- Network: network manager, DHCP, DNS resolver, firewall, SSH.
- Storage: mount service, disk manager, removable-media service.
- Desktop: Lumina compositor, Nova desktop shell, launcher, session services.
- Hardware: audio, Bluetooth, input, power, sensors.
- Maintenance: update service, diagnostics, monitoring.

## Folder Status

- `common`: shared service runtime helper for basic `--status`, `--run-once`, and `--help` daemon behavior.

- `shell`: buildable command shell.
- `login`: buildable text login path. GUI login code is parked until compositor
  integration is ready.
- `lumina`: compositor source, gated behind `TERRARIUM_BUILD_GUI_SERVICES`.
- `nova`: optional desktop shell boundary for taskbar, status, network tray,
  and workspaces. Low-level composition remains owned by Lumina.
- `audio`, `fs`, `input`, `launcher`, `log`, `net`, `time`: buildable daemon
  boundaries with simple status/start paths.

GUI services are optional. Lumina owns the Wayland/wlroots-style compositor
side, while Nova can be built separately as the desktop shell surface on
Terrarium UI.

## Services Work Queue

Priorities focus on services needed by login, shell, utilities, desktop startup,
networking, and hardware.

### High Priority

- Replace dummy login authentication in `login/src/auth.c`. It currently accepts
  only a demo user path; add real user database, password/shadow validation,
  account policy, failed-login handling, and integration with the system auth
  APIs.
- Finish session startup in `login/src/session.c`. The current code hardcodes
  `HOME`, `SHELL`, profile paths, Lumina, and shell execution. Add user-specific
  environment setup, privilege separation, session tokens, service registration,
  and clean failure handling.
- Implement the network service group. `net/firewall/main.c`, `net/netd/main.c`,
  and `net/sshd/main.c` are effectively empty; `net/dhcpd/dhcpd.c` only reports
  ready. Add NIC discovery, DHCP, DNS, routing, sockets, firewall rules, and SSH
  service integration.
- Turn core daemon boundaries into real long-running services. `audio`, `fs`,
  `input`, `launcher`, `log`, and `time` currently mostly expose ready/status
  behavior. Add event loops, IPC endpoints, `/run/...` control sockets,
  persistent state, restart behavior, and service-manager hooks.
- Provide the backend endpoints expected by utilities: `/run/services/control`,
  `/run/power/*`, `/run/security/*`, `/run/net/*`, `/proc/modules`,
  `/proc/net/*`, `/proc/kmsg`, `/proc/trace`, and mount/module control paths.

### Medium Priority

- Finish Lumina as a real compositor service. `--run` currently performs one
  cycle and exits. Add persistent compositor loop, Wayland client lifecycle,
  output hotplug, input routing, scene graph updates, damage tracking,
  decorations, and window management.
- Complete tiny Lumina source files such as `lumina/src/decorations.c` and
  `lumina/src/surface.c`.
- Finish Nova desktop shell integration. Several Nova headers are skeletal
  (`cursor.h`, `damage.h`, `input.h`, `layer_shell.h`, `output.h`, `seat.h`,
  `view.h`, and `protocol-client.h`). Add compositor connection, layer-shell
  support, input/output handling, view tracking, and status providers.
- Expand the shell service. The shell can run commands and a few builtins, but
  still needs job control, pipelines, redirects, variables, globbing, history,
  completion, process groups, and signal handling.
- Add Bluetooth service implementation. `bluetooth/main.c` is effectively empty
  and needs device discovery, pairing, profiles, and service control.

### Low Priority

- Rename `terrarium-service-placeholders` once the shared service headers are
  backed by real service client libraries.
- Replace the built-in `terrarium-init` unit table with service configuration files and expand service unit metadata so init can start
  services with dependencies and restart policies.
- Add diagnostics and status commands for each service beyond simple `--status`
  output.
