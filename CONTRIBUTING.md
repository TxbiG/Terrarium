# Contributing to Terrarium OS

Thank you for contributing to **Terrarium OS**, an experimental operating-system project designed around simplicity and ease of use.

Terrarium is a low-level project. Contributions should favour correctness, understandable architecture, portability, and small, reviewable changes.

## Before You Start

For substantial architectural changes, open an issue before implementation.

This is especially useful for changes involving:

- Kernel architecture
- Memory management
- Scheduling and processes
- Drivers
- Boot/platform support
- Filesystem design
- ABI/API changes
- Privilege or security boundaries

Please search existing issues and pull requests before starting work.

## Development Requirements

Terrarium uses CMake-based build infrastructure and low-level C code.

You should have:

- Git
- CMake
- A C/C++ toolchain appropriate to the target
- Any required cross-compilation tools
- An emulator or suitable test hardware where practical

A successful host build is not by itself proof that the operating system works correctly on its intended target.

## Building

```bash
git clone https://github.com/TxbiG/Terrarium.git
cd Terrarium

cmake -S . -B build
cmake --build build
```

Use the repository's documented presets/scripts for target-specific boot-image or emulator workflows.

## Repository Structure

- `boot/` — boot-related components
- `kernel/` — kernel code
- `lib/` — libraries
- `services/` — system services
- `system/` — system components
- `utilities/` — utilities
- `apps/` — applications
- `resources/` — system resources
- `Documentation/` — documentation
- `Scripts/` — development/build scripts
- `.github/workflows/` — CI

## Areas for Contribution

Contributions may include:

- Kernel development
- Memory management
- Scheduling, processes, and synchronisation
- Device drivers
- Filesystems
- Networking
- Graphics and display
- Input
- System services
- Applications and utilities
- Build/boot tooling
- Hardware and platform support
- Tests and debugging tools
- Documentation

## Architecture Guidelines

Keep OS layers clearly separated.

When adding functionality, consider whether it belongs in:

1. Boot/platform code
2. Kernel
3. Core library
4. System service
5. User-space utility/application

Avoid moving policy into low-level mechanisms when it can remain in a higher layer.

Hardware-specific behaviour should be isolated behind suitable interfaces wherever practical.

## Hardware and Platform Changes

Pull requests affecting hardware should state:

- Target architecture
- Board, device, or emulator
- Toolchain
- Build configuration
- How the change was tested
- Whether real hardware was tested

If only compilation, static analysis, or emulation was possible, state that clearly.

## Testing

Useful validation includes:

- Host-side unit tests where possible
- Cross-compilation
- Emulator boot tests
- Kernel smoke tests
- Driver tests
- Application-level tests
- Static analysis

Changes to memory management, interrupts, scheduling, synchronisation, or drivers should include a focused regression test or reproducible validation case when practical.

## Commit Messages

Recommended prefixes:

```text
feat: add framebuffer service
fix: correct page-table initialization
docs: document kernel memory layout
test: add scheduler regression test
refactor: isolate platform timer code
build: improve cross compilation
ci: add emulator smoke test
```

## Pull Requests

A good pull request should:

- Explain the problem.
- Explain the solution.
- Identify affected OS layers.
- Include tests or validation steps.
- State the target platform.
- Avoid unrelated changes.
- Document user-visible or architectural changes.

For kernel changes, include enough technical detail for another contributor to review the design safely.

## Reporting Bugs

Include:

- Commit/version
- Target architecture
- Hardware or emulator
- Toolchain
- Build configuration
- Exact reproduction steps
- Expected behaviour
- Actual behaviour
- Logs, panic output, serial output, screenshots, or traces where useful

## Security

Kernel, driver, boot, and privilege-boundary vulnerabilities should be reported privately when public disclosure could create an avoidable security risk.

Do not post credentials or private hardware information in public issues.

## Licence

Terrarium OS is distributed under the **MIT License**. Contributions should be compatible with the repository's licence and applicable third-party licence requirements.

Thank you for helping develop Terrarium OS.
