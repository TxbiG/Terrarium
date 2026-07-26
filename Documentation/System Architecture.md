# System Architecture

This document describes the high-level architecture of the operating system, including the kernel design, filesystem layout, and core system management subsystems. The OS is designed to scale across PCs, servers, consoles, phones, tablets, and embedded devices.


## Bootloader Overview
- Boot sequence (e.g. diagram)
---

## Kernel Overview

The operating system uses a **hybrid kernel architecture**.

The kernel provides:
- Core process and thread scheduling
- Virtual memory management
- Inter-process communication (IPC)
- Low-level hardware abstraction
- Security and privilege enforcement

Some subsystems (such as drivers and filesystems) may run:
- **In-kernel** for performance-critical paths
- **In user space** as modules or services for stability and portability

This hybrid approach balances:
- **Monolithic performance**
- **Microkernel-style modularity and fault isolation**

---

## File System Layout

The filesystem is structured to clearly separate the kernel, system libraries, services, and applications.

This layout ensures:
- Clear API boundaries
- Hardware independence
- Easier porting to new platforms

---

## Process & Thread Management

The kernel provides native support for:
- Processes with isolated address spaces
- Lightweight kernel threads
- User-space threading libraries (`libthread`)

Features include:
- Preemptive multitasking
- Priority-based scheduling
- Support for multi-core and SMP systems
- User and kernel thread separation

High-level services and applications use user-space threading libraries, while the kernel enforces scheduling and isolation.

---

## Memory Management

Memory management is handled by the kernel and includes:
- Virtual memory with per-process address spaces
- Demand paging and lazy allocation
- Memory protection and access control
- Shared memory for IPC and high-performance graphics

User-space libraries (such as `liballoc`) provide:
- Heap allocation
- Memory pools
- Optimized allocators for different workloads

This design supports:
- Desktop and mobile environments
- High-performance servers
- Low-memory embedded devices

---

## Device Management

Hardware devices are managed using a layered approach:

- **Kernel layer**
  - Hardware discovery
  - Interrupt handling
  - Low-level drivers
- **User-space services**
  - Device policy and permissions
  - Hot-plug handling
  - Power and performance control

Examples of device-related services:
- Input devices (keyboard, mouse, touch)
- Graphics and display devices
- Audio and camera devices
- Power and battery management
- Network interfaces

Applications interact with hardware through stable user-space libraries, never directly with drivers, ensuring security and portability.
