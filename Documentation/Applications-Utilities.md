# Phase 9 Applications and Utilities

Phase 9 makes shipped applications intentional and moves command-line tools onto
system APIs or explicit kernel/service endpoints.

## App build policy

`apps/CMakeLists.txt` now separates applications into three groups:

- `TERRARIUM_BUILD_SAMPLE_APPS`: top-level app build gate.
- `TERRARIUM_BUILD_BASE_APPS`: apps selected for the base image.
- `TERRARIUM_BUILD_EXPERIMENTAL_APPS`: prototype apps that still contain TODO
  rendering, direct Wayland experiments, or incomplete service dependencies.

The base app manifest is installed from `apps/base-apps.manifest` to
`/etc/terrarium/apps`.

Current base app status:

- Calculator: ported to `terrarium-app-runtime` and the shared
  `terrarium-window-client` lifecycle.
- Terminal: ported to `terrarium-app-runtime` with scrollback and explicit
  shell/PTY backend readiness; still blocked on real PTYs, shell service
  sessions, process groups, copy/paste, and full scrollback rendering.
- File Manager: ported to `terrarium-app-runtime` with VFS, mount, and block
  device readiness probes; still blocked on real directory listing, file
  operations, permissions, icons, thumbnails, and removable-device events.
- Task Manager: ported to `terrarium-app-runtime` with system API probes for
  hardware, process, and service state; still blocked on live `/proc`, scheduler,
  memory, and service state exports.
- Notes: ported to `terrarium-app-runtime` with UTF-8 validation and VFS
  readiness probes; still blocked on multiline text widgets, real file
  open/read/write/fsync contracts, clipboard, and rich text rendering.
- Settings: ported to `terrarium-app-runtime` with hardware/service/network
  readiness probes; still blocked on a settings daemon, policy store, panel
  schemas, and privileged apply/rollback APIs.

## App runtime boundary

`lib/Terra/app/app.h` defines the first Terrarium-owned app runtime wrapper:

- app metadata (`id`, name, kind, initial size)
- window-client initialization and disconnect
- display event handoff
- present/frame accounting
- close/shutdown lifecycle

This does not replace real SHM/EGL buffers, text rendering, icons, or compositor
input. It gives apps a common lifecycle while Lumina and the renderer mature.

## Utility policy

Utilities should prefer `system/*` APIs and fail clearly when a backend is not
implemented. New planned utilities were added with explicit endpoint messages:

- `route`
- `caps`
- `auditctl`
- `sandboxctl`
- `swapctl`
- `pmap`
- `slabtop`
- `drvbind`
- `drvunbind`
- `drvinfo`
- `hotplug`
- `initctl`
- `servicectl`
- `diagnostics`

These commands are intentionally thin until the corresponding kernel, `/proc`,
`/sys`, `/run`, service, or security contract exists.

## Still missing

- Port the remaining graphical apps from direct Wayland/TODO prototypes to
  Terra app runtime plus Terra UI widgets.
- Add real compositor surfaces and SHM/EGL buffer drawing.
- Add text rendering, icon loading, focus handling, accessibility, and input
  dispatch for app widgets.
- Back Task Manager with live process/scheduler/memory/service data.
- Back File Manager with VFS mutation APIs, thumbnails, mounts, permissions, and
  removable-device events.
- Back Terminal with PTYs, shell service sessions, job control, copy/paste, and
  scrollback.
- Replace each utility endpoint stub as soon as its backing API becomes real.




