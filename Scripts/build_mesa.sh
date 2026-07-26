#!/bin/bash

# OpenGL & Vulkan

# radv for AMD
# anv for Intel
# libvulkan (CLOSED SOURCE)

# I’ll need DRM/KMS support and PCI drivers in kernel

# Set your install prefix
--prefix=$HOME/Terrarium/root/usr

meson setup build/ \
  --prefix="$PREFIX" \
  -Dglx=disabled \
  -Degl=true \
  -Dgles2=true \
  -Dopengl=true \
  -Dgallium-drivers=swrast \
  -Dvulkan-drivers=lavapipe \
  -Dllvm=true \
  -Dshared-glapi=true \
  -Dbuildtype=release

ninja -C build          # Build Mesa
ninja -C build install  # Install to OS root

# Print success message
echo "✅ Mesa built and installed to $PREFIX"