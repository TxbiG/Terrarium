cmake_minimum_required(VERSION 3.20)
project(TerrariumOS C CXX)

option(ENABLE_DEBUG "Enable Debug" OFF)

# Platform properties
option(ENABLE_DESKTOP "Enable desktop build profile" OFF)
option(ENABLE_RASPBERRY_PI "Enable Raspberry Pi build profile" OFF)
option(ENABLE_SERVER "Enable server build profile" OFF)
option(ENABLE_EMBEDDED "Enable embedded build profile" OFF)
option(ENABLE_XR "Enable XR build profile" OFF)
option(TERRARIUM_BUILD_APPS "Build bundled graphical/sample applications" OFF)
option(TERRARIUM_BUILD_SMOKE_TESTS "Build small Terrarium contract smoke-test executables" OFF)
if(ENABLE_EMBEDDED)
    option(ENABLE_MOBILE "Enable mobile embedded profile" OFF)
    option(ENABLE_CONSOLE "Enable console embedded profile" OFF)
    option(ENABLE_TABLET "Enable tablet embedded profile" OFF)
    option(ENABLE_TV "Enable TV embedded profile" OFF)
endif()

# Kernel properties
option(KERNEL_ARCH_X86_64 "Build kernel for x86_64" ON)
option(KERNEL_ARCH_X86 "Build kernel for x86" OFF)
option(KERNEL_ARCH_AARCH64 "Build kernel for AArch64" OFF)
option(KERNEL_ARCH_ARM32 "Build kernel for ARM32" OFF)
option(KERNEL_ARCH_RISCV64 "Build kernel for RISC-V 64" OFF)
option(KERNEL_ARCH_RISCV32 "Build kernel for RISC-V 32" OFF)

option(TERRA_SINGLE_BOOT "Build a direct TerrariumOS boot image" ON)
option(TERRA_MULTI_BOOT "Build a Terrarium boot-menu image" OFF)
set(TERRA_BOOT_ARCH "x86_64" CACHE STRING "Boot architecture: x86, x86_64, arm32, aarch64, riscv32, riscv64")

set(KERNEL_PAGE_SIZE 4096 CACHE STRING "Kernel page size in bytes")
set(KERNEL_VA_BITS 48 CACHE STRING "Virtual address width")

option(KERNEL_PREEMPTIVE "Enable preemptive scheduling" ON)
option(KERNEL_TICKLESS "Enable tickless kernel (NO_HZ)" ON)
set(KERNEL_HZ 1000 CACHE STRING "Kernel timer frequency")

option(KERNEL_SLAB_ALLOCATOR "Enable slab allocator" ON)
option(KERNEL_BUDDY_ALLOCATOR "Enable buddy allocator" ON)
option(KERNEL_HUGEPAGES "Enable huge page support" OFF)

option(KERNEL_IPC_SHM "Shared memory IPC" ON)
option(KERNEL_IPC_MSG "Message-based IPC" ON)
option(KERNEL_FUTEX "Fast userspace mutexes" ON)

option(KERNEL_FS_VFS "Virtual filesystem layer" ON)
option(KERNEL_FS_EXT "EXT-family filesystem" ON)
option(KERNEL_FS_FAT "FAT filesystem" ON)
option(KERNEL_FS_TMPFS "TMPFS support" ON)

option(KERNEL_DRIVER_PCI "PCI bus support" ON)
option(KERNEL_DRIVER_USB "USB support" ON)
option(KERNEL_DRIVER_BT "Bluetooth support" OFF)
option(KERNEL_DRIVER_WIFI "WiFi support" ON)
option(KERNEL_DRIVER_GPU "GPU / DRM support" ON)
option(KERNEL_DRIVER_AUDIO "Audio subsystem" ON)

option(KERNEL_AUDIO_RESON_ABI "Enable Reson audio ABI" ON)
option(KERNEL_AUDIO_MIXER "Kernel audio mixer support" OFF)

option(KERNEL_USER_MODE "Enable user mode" ON)
option(KERNEL_SYSCALL_FILTER "Syscall filtering" ON)
option(KERNEL_CAPABILITIES "Capabilities-based security" OFF)

option(KERNEL_DEBUG "Enable debug features" OFF)
option(KERNEL_KASAN "Kernel address sanitizer" OFF)
option(KERNEL_TRACE "Enable tracing framework" OFF)

option(KERNEL_SERIAL_CONSOLE "Enable serial console" ON)
option(KERNEL_EARLY_LOG "Enable early boot logging" ON)

option(KERNEL_UEFI "UEFI boot support" ON)
option(KERNEL_LEGACY_BIOS "Legacy BIOS support" OFF)
option(TERRARIUM_USE_UACPI "Enable the bundled uACPI integration boundary" OFF)

option(TERRARIUM_ENABLE_MESA "Enable Mesa/OpenGL/Vulkan userspace integration from a prebuilt Terrarium sysroot prefix" OFF)
set(TERRARIUM_MESA_ROOT "" CACHE PATH "Absolute path to a Mesa install prefix/sysroot for TerrariumOS")
option(TERRARIUM_MESA_OPENGL "Import Mesa EGL/OpenGL/GLES libraries when TERRARIUM_ENABLE_MESA is ON" ON)
option(TERRARIUM_MESA_VULKAN "Import Vulkan loader/ICD assets when TERRARIUM_ENABLE_MESA is ON" ON)

add_subdirectory(boot)
add_subdirectory(kernel)
add_subdirectory(lib)
add_subdirectory(services)
add_subdirectory(system)
if(TERRARIUM_BUILD_APPS)
    add_subdirectory(apps)
endif()
add_subdirectory(utilities)
if(TERRARIUM_BUILD_SMOKE_TESTS)
    add_subdirectory(smoke)
endif()
add_custom_target(terrarium-build-smoke
    COMMENT "Configure/build smoke target for core TerrariumOS boundaries"
)
add_dependencies(terrarium-build-smoke
    terrarium_boot_config
    kernel.elf
    terrarium-lib
    terrarium-system
    terrarium-services
    terrarium-utilities
)

if(TARGET terrarium-contract-smoke)
    add_dependencies(terrarium-build-smoke terrarium-contract-smoke)
endif()
