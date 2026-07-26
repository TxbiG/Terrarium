# ADR 0004: Graphics Import Boundary

Status: accepted

## Context

TerrariumOS wants EGL, OpenGL, GLES, Vulkan, Wayland, and wlroots integration,
but importing those projects as if they were Terrarium-owned source would blur
maintenance and platform responsibilities.

## Decision

Mesa, Wayland, wlroots, stb, and similar third-party code stay behind explicit
vendor/import boundaries. The first Mesa profile is software-rendering-first;
hardware rendering waits for the kernel graphics ABI to provide render devices,
buffer allocation/import/export, mmap metadata, command submission, fences, and
permissions.

## Consequences

- Terrarium-owned work lives in platform glue, ABI definitions, compositor
  integration, and app/runtime contracts.
- Prebuilt Mesa import remains separate from Terrarium source ownership.
- Hardware Mesa support is blocked on kernel GPU/render-node implementation.
