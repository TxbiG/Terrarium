# Terrarium Documentation

## Overview
- [What is Terrarium?](#what-is-terrarium)
- [System Requirements](#system-requirements)
- [Supported Architectures](#supported-architectures)
- [Licensing](/LICENSE)

## Getting Started
- [Installation Guide](/Documentation/Getting%20Started.md#installation-guide)
- [First Boot & Setup](/Documentation/Getting%20Started.md#first-boot--setup)
- [Recovery / Safe Mode](/Documentation/Recovery.md)
- [Updating the OS](/Documentation/Packaging-Updates-Recovery.md)

## System Architecture
- [Kernel Overview](/Documentation/System%20Architecture.md#kernel-overview)
- [File System Layout](/Documentation/System%20Architecture.md#file-system-layout)
- [Process & Thread Management](/Documentation/System%20Architecture.md#process--thread-management)
- [Memory Management](/Documentation/System%20Architecture.md#memory-management)
- [Device Management](/Documentation/System%20Architecture.md#device-management)

## Core API
- [System Calls Reference](/Documentation/)
- [IPC & Signals](/Documentation/)
- [Filesystem API](/Documentation/)
- [Memory API](/Documentation/)
- [Networking API](/Documentation/)

## Driver API
- [Writing Drivers](/Documentation/)
- [Device Classes (USB, PCI, Audio, Video, etc.)](/Documentation/)
- [Kernel Modules](/Documentation/)
- [Driver Signing & Permissions](/Documentation/Security-Model.md)

## Libraries
- [Standard Library (C/C++ or other)](/Documentation/)
- [GUI Toolkit](/Documentation/)
- [Networking Stack](/Documentation/)
- [Math, String, IO Helpers](/Documentation/)
- [Encryption / Compression](/Documentation/)

## User Manual
- [Navigating the Desktop Environment](/Documentation/)
- [Using Workspaces & Windows](/Documentation/)
- [Settings, Shortcuts & Preferences](/Documentation/)
- [Power Management](/Documentation/)
- [Troubleshooting Common Issues](/Documentation/)
- [Utilities](/Documentation/Applications-Utilities.md)

## Developer Documentation
- [Testing Strategy](/Documentation/Testing.md)
- [Architecture Decision Records](/Documentation/ADR/README.md)
- [Packaging, Updates, Security, and Recovery](/Documentation/Packaging-Updates-Recovery.md)
- [Build Applications](/Documentation/)
- [Package Management](/Documentation/Package-Management.md)
- [System Customization](/Documentation/)
- [Community Contributions](/Documentation/)

## Programs
- Terminal
- File Browser
- Code Editor
- Notepad
- Word Processor
- Spreadsheet Editor
- Presentation Program
- Paint / Drawing App
- Multimedia Player(s)
- Mail Client
- PDF Editor
- Calculator
- Web Browser

## File Formats Supported
- **Documents:** `.txt`, `.docx`, `.odt`, `.pdf`, `.md`, `.rtf`
- **Spreadsheets:** `.xlsx`, `.ods`, `.csv`
- **Presentations:** `.pptx`, `.odp`
- **Media:** `.mp3`, `.wav`, `.flac`, `.mp4`, `.webm`, `.avi`, `.mkv`
- **Images:** `.png`, `.jpg`, `.bmp`, `.svg`, `.ico`, `.gif`
- **Archives:** `.zip`, `.tar`, `.gz`, `.7z`, `.rar`
- **Code:** `.c`, `.cpp`, `.py`, `.js`, `.html`, `.css`, `.sh`, `.json`, `.xml`, `.yaml`
- **Executable:** `.exe`, `.sh`, `.bin`, `.elf`


---

# What is Terrarium?

# System Requirements

# Supported Architectures

## Platform-Specific Architecture
The operating system is designed to support multiple device categories from a single codebase.
Platform differences are handled through hardware abstraction, modular drivers, and user-space services.

```text
TERRARIUM_ARM - ARM cpu arch
TERRARIUM_X86 - X86 cpu arch
TERRARIUM_DESKTOP - Computers or laptops
TERRARIUM_RASPBERRY_PI - 
TERRARIUM_XR - AR/MR/VR
PLATFORM_SERVER -
PLATFORM_EMBEDDED - embedded devices
PLATFORM_MOBILE - 
PLATFORM_CONSOLE - 
PLATFORM_TABLET - 
PLATFORM_TV - 
```
---

### Desktop & Laptop Computers (PC)

**Primary goals:**
- High performance
- Rich desktop environment
- Broad hardware compatibility

**Kernel configuration:**
- Full hybrid kernel
- Preemptive multitasking
- SMP and NUMA support
- Full virtual memory and paging

**Enabled services:**
- Display compositor and window manager
- Desktop environment
- Audio and multimedia services
- Power management
- Network and peripheral services

**Graphics stack:**
- Wayland compositor
- EGL + OpenGL / Vulkan
- GPU drivers via Mesa or vendor backends

**Input:**
- Keyboard, mouse, game controllers
- Touch and stylus (optional)

---

### Raspberry Pi & Embedded Devices

**Primary goals:**
- Low power usage
- Small memory footprint
- Hardware flexibility

**Kernel configuration:**
- Hybrid kernel with minimal in-kernel drivers
- Reduced scheduler complexity
- Optional swap or compressed memory

**Enabled services:**
- Headless mode (no compositor)
- Optional lightweight compositor
- GPIO and hardware control services
- Network and remote management services

**Graphics stack (optional):**
- Simple framebuffer or lightweight Wayland compositor
- EGL/OpenGL ES

**Use cases:**
- Kiosks
- IoT controllers
- Media players
- Educational devices

---

### Network Servers

**Primary goals:**
- Reliability
- Security
- High concurrency

**Kernel configuration:**
- Hybrid kernel optimized for I/O
- High-performance networking stack
- Large address space support

**Enabled services:**
- No desktop or compositor, just a basic shell
- Network, storage, and system services
- Logging, monitoring, and watchdog services

**Graphics stack:**
- Disabled or minimal framebuffer
- Remote management only

**Security features:**
- Strict privilege separation
- Mandatory access control
- Service isolation

---

### Mobile Devices (Phones/Tablets)

**Primary goals:**
- Power efficiency
- Touch-first interface
- Sensor integration

**Kernel configuration:**
- Hybrid kernel with aggressive power management
- Fast suspend/resume
- Sensor and modem support

**Enabled services:**
- Mobile compositor
- Touch and gesture service
- Telephony and sensor services
- Power and battery services

**Graphics stack:**
- Wayland compositor
- EGL + OpenGL ES / Vulkan
- Hardware video acceleration (vk_video)

**Input:**
- Multi-touch
- Sensors (accelerometer, gyroscope)
- On-screen keyboard

---

### Smart TVs & Consoles

**Primary goals:**
- Media playback
- Controller-based input
- Stable long-running sessions

**Kernel configuration:**
- Hybrid kernel with real-time optimizations
- Fast boot and resume

**Enabled services:**
- Media compositor
- Audio/video services
- Controller and remote input services
- Network and update services

**Graphics stack:**
- Full-screen Wayland compositor
- EGL + Vulkan
- Hardware video decode and encode

**Input:**
- Game controllers
- Remote controls
- Limited keyboard support

---

### Shared Architecture Across All Platforms

All platforms share:
- The same kernel core
- The same system libraries
- The same IPC mechanisms
- The same security and permission model

Platform-specific behavior is controlled through:
- Kernel configuration flags
- Modular drivers
- Selectively enabled services
- Platform-specific libraries in `libplatform/`

This ensures maximum code reuse while allowing device-specific optimization.

---



