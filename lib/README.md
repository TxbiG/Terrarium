# Library

This folder contains TerrariumOS userspace library headers, format headers,
and imported graphics/windowing API trees.

The core Terra UI surface now builds as `terrarium-ui`, and the lightweight
Wayland connection/protocol compatibility layer builds as `terrarium-wayland-lite`.

Source-heavy imported trees such as Mesa, full Wayland, and wlroots still need
TerrariumOS platform glue before they should be compiled as normal libraries.
Mesa can now be imported from a prebuilt Terrarium-compatible prefix; see
`Documentation/Mesa.md`.

## Library Work Queue

Priorities focus on what blocks applications, services, Lumina, Nova, and core
system tools from becoming usable.

### High Priority

- Finish the Terrarium-native system library layer in `Terra/system`. The
  current headers define ownership boundaries, but most areas still need real
  syscall/service-backed implementations for storage, modules, power, sensors,
  camera, Bluetooth, networking, and generic system calls.
- Replace or isolate Linux/ALSA-derived audio code in
  `Terra/system/audio/usbaudio.c` and `Terra/system/audio/jack.c`. These files
  still carry many FIXME/hack markers and need Terrarium-native audio device
  ABI integration, mixer/control handling, sample format conversion, and
  playback/capture synchronization.
- Finish power management in `Terra/system/power/power.c`. Shutdown currently
  behaves as a CPU halt placeholder, and hibernation is explicitly still TODO.
  Wire this through Terrarium kernel lifecycle, ACPI, and the power service.
- Finish the Wayland window client in
  `Terra/window/Display/Window/Window.c`. Event handling is still stubbed and
  needs xdg-shell configure, close, ping, resize, fullscreen, maximize, and
  dispatch behavior.
- Decide which imported graphics/windowing trees are vendor source and which
  are Terrarium build targets. Mesa now has a prebuilt-prefix import boundary
  for EGL/OpenGL/GLES/Vulkan, but full in-tree Mesa, Wayland, and wlroots still
  need platform, allocator, syscall, compositor, and device glue before
  compiling them as normal libraries.

### Medium Priority

- Finish the update client in
  `Terra/system/update-client/update-daemon/update-daemon.c`. Config parsing
  and installed-version lookup are currently stubs; add package database
  access, update source config, signature checks, rollback policy, and logging.
- Expand `terrarium-ui` beyond the currently compiled core widgets. Many UI
  headers under `Terra/gfx/UI` are skeletal and need state, layout, drawing,
  focus, event handling, and accessibility hooks.
- Finish network library APIs under `Terra/network`. DNS, HTTP, TLS, QUIC, and
  WebSocket headers exist, but they need implementations backed by Terrarium
  socket and service APIs.
- Add real ABI/client helpers for `Terra/system/ABI` and `Terra/system/syscall.h`
  so apps and services stop reaching directly into ad-hoc endpoints.

### Low Priority

- Clean duplicate or temporary UI files such as the copied `InputField` headers
  once the final input field API is settled.
- Normalize spelling and naming across UI headers, for example `Calender.h`.
- Review third-party TODO/FIXME markers in `stb`, Mesa, wlroots, and Wayland
  separately. Those are upstream/vendor tasks unless Terrarium needs a local
  platform patch.
