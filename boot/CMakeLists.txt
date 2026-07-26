cmake_minimum_required(VERSION 3.16)
project(terrarium_boot C ASM)

option(TERRA_SINGLE_BOOT "Boot directly into TerrariumOS" ON)
option(TERRA_MULTI_BOOT "Enable Terrarium boot menu" OFF)
option(TERRA_BOOT_ENABLE_BIOS_REALMODE_BINDINGS "Build BIOS real-mode interrupt binding shims" OFF)

set(TERRA_BOOT_ARCH "x86_64" CACHE STRING "Boot architecture: x86, x86_64, arm32, aarch64, riscv32, riscv64")
set_property(CACHE TERRA_BOOT_ARCH PROPERTY STRINGS x86 x86_64 arm32 aarch64 riscv32 riscv64)

set(TERRA_BOOT_PLATFORM_SOURCES)
set(TERRA_BOOT_ARCH_SOURCES)

if(TERRA_BOOT_ARCH STREQUAL "x86" OR TERRA_BOOT_ARCH STREQUAL "x86_64")
    list(APPEND TERRA_BOOT_ARCH_SOURCES arch/x86/boot_jump.S)
    list(APPEND TERRA_BOOT_PLATFORM_SOURCES platform/x86_platform.c platform/x86_firmware_io.c platform/x86_bios_protocol.c platform/x86_uefi_protocol.c platform/x86_bios_realmode.c platform/x86_uefi_bindings.c)
    if(TERRA_BOOT_ENABLE_BIOS_REALMODE_BINDINGS)
        list(APPEND TERRA_BOOT_ARCH_SOURCES arch/x86/bios_realmode.S)
    endif()
    if(KERNEL_UEFI)
        list(APPEND TERRA_BOOT_PLATFORM_SOURCES platform/uefi.c platform/uefi_entry.c)
    else()
        list(APPEND TERRA_BOOT_PLATFORM_SOURCES platform/x86_bios.c)
    endif()
elseif(TERRA_BOOT_ARCH STREQUAL "aarch64" OR TERRA_BOOT_ARCH STREQUAL "arm32")
    list(APPEND TERRA_BOOT_ARCH_SOURCES arch/arm/64/boot_jump.S)
    if(KERNEL_UEFI)
        list(APPEND TERRA_BOOT_PLATFORM_SOURCES platform/uefi.c)
    else()
        list(APPEND TERRA_BOOT_PLATFORM_SOURCES platform/aarch64_firmware.c)
    endif()
elseif(TERRA_BOOT_ARCH STREQUAL "riscv32" OR TERRA_BOOT_ARCH STREQUAL "riscv64")
    list(APPEND TERRA_BOOT_ARCH_SOURCES arch/riscv/boot_jump.S)
    list(APPEND TERRA_BOOT_PLATFORM_SOURCES platform/riscv_sbi.c)
endif()

add_library(terrarium_boot_config STATIC
    Bootloader.c
    core/boot_config.c
    core/handoff.c
    core/platform_common.c
    core/gfx/GUI.c
    ${TERRA_BOOT_PLATFORM_SOURCES}
    ${TERRA_BOOT_ARCH_SOURCES}
)

target_include_directories(terrarium_boot_config PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../kernel/include
)

if(TERRA_MULTI_BOOT)
    target_compile_definitions(terrarium_boot_config PUBLIC TERRA_MULTI_BOOT=1)
else()
    target_compile_definitions(terrarium_boot_config PUBLIC TERRA_SINGLE_BOOT=1)
endif()

target_compile_definitions(terrarium_boot_config PUBLIC
    TERRA_BOOT_ARCH="${TERRA_BOOT_ARCH}"
)

if(TERRA_BOOT_ARCH STREQUAL "x86" OR TERRA_BOOT_ARCH STREQUAL "x86_64")
    target_compile_definitions(terrarium_boot_config PUBLIC TERRA_BOOT_X86_PLATFORM=1)
endif()

if(TERRA_BOOT_ENABLE_BIOS_REALMODE_BINDINGS)
    target_compile_definitions(terrarium_boot_config PUBLIC TERRA_BOOT_ENABLE_BIOS_REALMODE_BINDINGS=1)
endif()







if(KERNEL_UEFI)
    target_compile_definitions(terrarium_boot_config PUBLIC TERRA_BOOT_UEFI_PLATFORM=1)
endif()

