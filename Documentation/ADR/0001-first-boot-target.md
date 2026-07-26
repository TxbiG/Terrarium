# ADR 0001: First Supported Boot Target

Status: accepted

## Context

The tree contains architecture placeholders for several targets, but the current
boot documentation and scripts point most clearly at an x86_64 emulator path.
Trying to bring up all architecture/firmware combinations at once would hide
handoff bugs and make testing noisy.

## Decision

TerrariumOS will use x86_64 emulator boot as the first real target. Other
architectures remain documented placeholders until their firmware paths are
actively tested.

The first target owns:

- boot image staging
- kernel handoff validation
- framebuffer and memory-map discovery
- emulator command documentation
- initial integration testing

## Consequences

- Early boot work should prefer x86_64 correctness over cross-architecture
  breadth.
- ARM, AArch64, and RISC-V stubs should fail clearly or remain gated.
- Hardware matrices are postponed until emulator boot is stable.
