# Phase 7 Graphics and Windowing

Phase 7 starts by making the display stack boundaries explicit before wiring
real Wayland, wlroots, Mesa, or GPU backends into normal boot.

## Current contracts

- `Documentation/Mesa.md` defines the first supported Mesa profile: prebuilt
  Mesa import with software rendering first, hardware acceleration later.
- `kernel/drivers/gpu/gpu.h` names the first hardware Mesa ABI boundary:
  render device discovery, buffer metadata, export/import, mmap metadata, cache
  policy, fences, and command submission. The current implementation is a
  conservative metadata stub; framebuffer/software devices still report command
  submission as unsupported.
- `lib/Terra/window/display_protocol.h` defines the shared display event,
  output, and view metadata structs.
- `lib/Terra/window/desktop_protocol.h` defines the first compositor-adjacent
  protocol structs for layer-shell-style surfaces, screenshots, recording,
  clipboard, drag/drop, accessibility nodes, and notifications.
- `lib/Terra/window/window_client.h` and `window_client.c` provide the first clean
  client-side window state API, built as `terrarium-window-client`.
- `lib/Terra/window/window.h` is now a C-safe API surface, and
  `Display/Window/Window.c` contains an opt-in Wayland/xdg-shell client
  implementation for registry binding, ping/pong, configure/ack, close, resize,
  maximize, fullscreen, event dispatch, and redraw commits. Build it with
  `TERRARIUM_BUILD_WAYLAND_WINDOW=ON` when the Wayland client stack is ready.
- Lumina includes the display protocol and tracks frame/event counters, a
  bounded in-memory event ring, damage rectangles, and pending frame callbacks
  in its compositor state.
- Lumina emits placeholder output-added, view-created, view-focused,
  view-destroyed, configure, close-request, and frame events so later
  IPC/Wayland plumbing has an obvious seam.
- Lumina tracks xdg-style configure serials. A view starts unmapped after
  creation, becomes mapped only after `lumina_xdg_ack_configure`, and can
  receive a close request before final destroy.
- `terrarium-lumina --frames N` runs a bounded compositor loop for smoke tests.
- `terrarium-lumina --key KEYCODE [up|down]` exercises input routing into the
  compositor event ring and keybinding layer.
- `terrarium-lumina --configure ID W H --ack` exercises configure/ack/mapping.
- `terrarium-lumina --request-close ID` emits a close request without destroying
  the view immediately.
- `terrarium-lumina --close ID` exercises view destroy/focus-next behavior.
- Nova now has a protocol-client boundary and reports its compositor connection
  state, protocol version, taskbar layer, launcher layer, status providers, and
  workspaces.

## Still missing

- Real Lumina Wayland socket/client lifecycle.
- Real xdg-shell object lifecycle over an IPC transport.
- Shared-memory or EGL buffer import.
- Input event routing from kernel/input services.
- Output hotplug and mode setting.
- Real render-node drivers, mmap-backed GPU memory, hardware queues, and fence
  wakeups.
- Full Nova launcher, tray, app switching, layer-shell negotiation, and
  persistent compositor connection.
- Terra UI drawing/input/focus/accessibility backends.
- Implementations for screenshot/screen-recording, clipboard, drag/drop,
  accessibility, and notification protocols.

Keep Mesa, Wayland, and wlroots vendor code separated from Terrarium glue. Only
add Terrarium-owned platform glue when a kernel/service contract exists for it.

## Smoke tests

Set `TERRARIUM_BUILD_SMOKE_TESTS=ON` to build `window-client-smoke`, a small
contract executable that validates the `terrarium-window-client` state machine
without requiring a real Wayland socket.

