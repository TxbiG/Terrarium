set(TERRARIUM_KERNEL_SOURCES
    main.c
    lifecycle.c
    bootloader.c
    arch/arch.c
    arch/acpi.c
    arch/x86/x86_init.c
    arch/arm/arm_init.c
    arch/riscv/riscv_init.c
    interrupt/interrupt.c
    ipc/ipc.c
    fs/fs.c
    fs/fs_types.c
    fs/blockdev.c
    fs/diskfs.c
    fs/exfat/exfat.c
    fs/ext4/ext4.c
    fs/ntfs3/ntfs3.c
    fs/xfs/xfs.c
    fs/vfs_core.c
    drivers/drivers.c
    drivers/device.c
    drivers/audio/audio.c
    drivers/camera/camera.c
    drivers/gpu/gpu.c
    drivers/input/input.c
    drivers/nics/nic.c
    drivers/nics/nic_controller.c
    drivers/nics/e1000.c
    drivers/nics/ne2k.c
    drivers/nics/rtl8139.c
    drivers/nics/rtl8169.c
    drivers/storage/storage.c
    drivers/storage/ata.c
    drivers/storage/nvme.c
    security/security.c
    syscalls/syscall_table.c
    time/timekeeper.c
    process/process.c
    module/module.c
    net/net.c
    mm/heap.c
    mm/MemoryManager.c
    mm/mm.c
    mm/paging.c
    mm/physical_mem.c
    mm/swap.c
    drivers/bus/PCI.c
    drivers/bus/Serial.c
    drivers/bus/VirtIO.c
)

add_executable(kernel.elf
    ${TERRARIUM_KERNEL_SOURCES}
)

target_include_directories(kernel.elf PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/arch
    ${CMAKE_CURRENT_SOURCE_DIR}/interrupt
    ${CMAKE_CURRENT_SOURCE_DIR}/ipc
    ${CMAKE_CURRENT_SOURCE_DIR}/fs
    ${CMAKE_CURRENT_SOURCE_DIR}/drivers
    ${CMAKE_CURRENT_SOURCE_DIR}/security
    ${CMAKE_CURRENT_SOURCE_DIR}/syscalls
    ${CMAKE_CURRENT_SOURCE_DIR}/time
    ${CMAKE_CURRENT_SOURCE_DIR}/process
    ${CMAKE_CURRENT_SOURCE_DIR}/module
    ${CMAKE_CURRENT_SOURCE_DIR}/net
    ${CMAKE_CURRENT_SOURCE_DIR}/mm
    ${CMAKE_CURRENT_SOURCE_DIR}/external/uACPI/include
)

target_compile_definitions(kernel.elf PRIVATE
    $<$<BOOL:${KERNEL_ARCH_X86_64}>:KERNEL_ARCH_X86_64=1>
    $<$<BOOL:${KERNEL_ARCH_X86}>:KERNEL_ARCH_X86=1>
    $<$<BOOL:${KERNEL_ARCH_AARCH64}>:KERNEL_ARCH_AARCH64=1>
    $<$<BOOL:${KERNEL_ARCH_ARM32}>:KERNEL_ARCH_ARM32=1>
    $<$<BOOL:${KERNEL_ARCH_RISCV64}>:KERNEL_ARCH_RISCV64=1>
    $<$<BOOL:${KERNEL_ARCH_RISCV32}>:KERNEL_ARCH_RISCV32=1>
    $<$<BOOL:${TERRARIUM_USE_UACPI}>:TERRARIUM_USE_UACPI=1>
)

target_compile_options(kernel.elf PRIVATE
    $<$<COMPILE_LANGUAGE:C>:-ffreestanding>
    $<$<COMPILE_LANGUAGE:C>:-fno-builtin>
)

target_link_options(kernel.elf PRIVATE
    -nostdlib
)
