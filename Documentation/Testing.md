# Testing Strategy

Terrarium tests should prove contracts before they depend on a complete bootable
system. Early tests are grouped by how much OS machinery they require.

## Smoke targets

`terrarium-contract-smoke` is the default build-time smoke target. It should stay
small, deterministic, and free of emulator requirements.

Current smoke executables:

- `window-client-smoke`: validates the Terra window-client lifecycle and display
  event handling.
- `phase10-contract-smoke`: validates package manifest rules, signature policy
  defaults, capability bit helpers, update/recovery status APIs, and explicit
  backend-not-ready behavior.

## Unit-level contracts

Prefer unit tests for pure logic and boundary validation:

- boot config parsing and boot-info checksum validation
- VFS path validation and mount option parsing
- package manifests, repository metadata, dependency solve summaries, and
  transaction state validation
- signature policy defaults and revocation decisions
- text/config/UTF-8 helpers
- utility argument parsing and backend-missing behavior

## Emulator integration tests

Emulator tests should start only after the x86_64 boot path reaches a stable
kernel log. Initial scenarios should be:

1. boot to kernel log
2. boot to init service manager
3. mount `tmpfs`, `devfs`, and `procfs`
4. run shell command through PTY
5. query service status
6. start Lumina/Nova enough to create a compositor connection
7. run a base app smoke launch

## Hardware matrix

Hardware testing comes after emulator success. The first matrix should record:

- firmware: UEFI or BIOS
- CPU architecture and core count
- framebuffer/GOP mode
- storage controller
- NIC driver
- input devices
- GPU/render path
- suspend/reboot behavior

## Rules

- A smoke test may assert `TERRA_STATUS_NOTREADY` when the missing backend is the
  expected current contract.
- Do not add host fallbacks to make a test pass.
- Keep tests linked against public `system`, `lib`, or documented kernel
  contract headers instead of private subsystem internals unless the test is
  explicitly for that subsystem.
