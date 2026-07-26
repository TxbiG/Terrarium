# Mesa, OpenGL, and Vulkan

TerrariumOS imports Mesa through a prebuilt sysroot/prefix. The repository may
contain Mesa source/header snapshots, but Terrarium-owned builds should treat
Mesa as a third-party provider until the kernel graphics ABI and compositor
platform glue are stable.

## Supported profile

The first supported Mesa profile is software rendering:

- EGL/OpenGL/GLES through a prebuilt Mesa prefix.
- Lavapipe/llvmpipe style software rendering before hardware GPU drivers.
- Vulkan loader/ICD discovery from the same prefix, with the expectation that
  the selected ICD is software-capable on TerrariumOS.
- No in-tree Mesa hardware driver build is considered supported yet.

Hardware acceleration comes later, after render-node permissions, GPU memory,
command submission, fences, and compositor buffer sharing are stable.

## Configure

Build or stage Mesa into a Terrarium-compatible prefix, then point CMake at it:

```powershell
cmake -S . -B build `
  -DTERRARIUM_ENABLE_MESA=ON `
  -DTERRARIUM_MESA_ROOT=C:/path/to/terrarium-mesa-prefix
```

The root path must be absolute. CMake searches only under that prefix for:

- `include/EGL/egl.h`
- `include/vulkan/vulkan.h`
- EGL/OpenGL/GLESv2 libraries
- the Vulkan loader library
- optional Vulkan ICD manifests under `share/vulkan`

Imported targets exposed to TerrariumOS code:

- `Terrarium::Mesa`
- `Terrarium::EGL`
- `Terrarium::OpenGL`
- `Terrarium::GLESv2`
- `Terrarium::Vulkan`

The umbrella `terrarium-lib` target links `Terrarium::Mesa` when
`TERRARIUM_ENABLE_MESA` is enabled.

## Kernel graphics ABI boundary

`kernel/drivers/gpu/gpu.h` now defines ABI version `TERRA_GPU_ABI_VERSION` and
names the hardware-facing contracts Mesa will eventually need:

- render device discovery with `gpu_render_device_count` and
  `gpu_render_device_info`
- buffer allocation, export/import, mmap metadata, cache policy, and destroy
- fence creation, signal, and wait
- command submission through `gpu_submit`
- permission-relevant buffer flags for CPU/GPU read/write, scanout, and sharing

The current implementation is intentionally conservative. Boot framebuffer and
software-render devices can be discovered and can own buffer metadata, but they
report `supports_command_submission = 0`. `gpu_submit` therefore fails until a
real render-node driver registers command submission support.

## Not supported yet

- DRM/GEM-compatible ioctl surface.
- Real mmap-backed GPU buffer memory.
- Hardware command queues.
- Timeline fences or cross-process sync objects.
- EGL/Vulkan window-system surface glue for Lumina.
- Direct wlroots/Mesa platform integration.

Until those pieces exist, Vulkan headers or a Vulkan loader alone do not provide
a working hardware Vulkan implementation. The loader can discover ICD manifests,
but the ICD still needs a software or hardware driver that can run on
TerrariumOS.
