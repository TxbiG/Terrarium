# Vendor Source Policy

TerrariumOS keeps third-party and imported source trees separate from Terrarium-owned implementation work.

Vendor trees currently include Mesa/EGL sources, Wayland, wlroots, stb, and uACPI. TODO, FIXME, stub, and compatibility notes inside those trees should be treated as upstream/vendor notes unless TerrariumOS needs a local platform boundary, build integration, ABI adapter, or security patch.

## Rules

- Do not mix Terrarium platform glue directly into vendor code when a wrapper, adapter, CMake import boundary, or small local patch layer can hold the change.
- Track Terrarium-owned work in Terrarium docs and work queues, not by rewriting upstream TODOs.
- Keep imported source builds gated until their allocator, syscall, threading, filesystem, device, and window-system dependencies are explicit.
- Prefer prebuilt/sysroot import boundaries for large graphics stacks until the kernel and user ABI can support native drivers.
- When a vendor patch is unavoidable, document the reason, affected upstream files, and the Terrarium API or ABI it depends on.

## Current Boundaries

- Mesa: imported through a prebuilt prefix via `TERRARIUM_ENABLE_MESA`; in-tree hardware driver builds wait for a kernel graphics ABI.
- Wayland/wlroots: source trees are present, but only lightweight Terrarium-owned boundaries should be linked until compositor and platform glue are ready.
- stb: header-only vendor libraries; local changes should be minimal and documented.
- uACPI: available behind `TERRARIUM_USE_UACPI`; Terrarium-owned work should focus on required platform callbacks.