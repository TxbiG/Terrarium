# TerrariumOS Applications

Applications are now split by shipping confidence instead of compiling every
prototype by default.

## Build options

- `TERRARIUM_BUILD_SAMPLE_APPS`: master switch for this directory.
- `TERRARIUM_BUILD_BASE_APPS`: builds base-image apps that have been ported to
  Terrarium-owned runtime/window contracts.
- `TERRARIUM_BUILD_EXPERIMENTAL_APPS`: builds older prototypes that may still
  contain direct Wayland experiments, TODO rendering paths, or missing service
  dependencies.

The root project still gates this directory with `TERRARIUM_BUILD_APPS`.

## Base image manifest

`base-apps.manifest` records the apps intended for the base image and their
porting status. Calculator, Terminal, Task Manager, File Manager, Notes, and Settings are
the first ported base apps and use `lib/Terra/app/app.h` plus the shared
window-client lifecycle. Terminal, Task Manager, File Manager, Notes, and Settings still
report missing shell/PTY/process/service/VFS/text/clipboard/settings-policy backends clearly
where those OS contracts are not implemented yet.

Do not move an app from experimental to base until it has:

- a bounded app lifecycle,
- compositor/window-client integration,
- no infinite TODO render loops,
- clear backend errors for missing OS services,
- and install metadata in `base-apps.manifest`.





