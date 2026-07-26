# Phase 10 Packaging, Updates, Security, and Recovery

Phase 10 makes TerrariumOS maintainable after installation. The current work
adds stable user-space contracts first, with explicit backend-not-ready results
where storage, networking, cryptography, service timers, or privileged policy
engines are not implemented yet.

## Package format

Terrarium packages use the `TPKG` format boundary with
`TERRA_PACKAGE_FORMAT_VERSION == 1`.

The public manifest model records:

- package identity: name, version, architecture, kind, description
- content hash and installed-size metadata
- dependency constraints with optional dependencies
- signature identity
- rollback group

The install model separates dependency solving from mutation:

1. solve package dependencies and conflicts
2. prepare a named transaction
3. commit the transaction
4. rollback the same transaction if validation or boot health fails

The implementation now includes a hosted bootstrap package database, manifest reader, dependency-solve summary, transaction prepare/commit/rollback state, and package list/query behavior. Real archive unpacking, durable install database storage, and filesystem mutation are still pending.

## Repository and update flow

Repositories carry channel, TLS-required, signature-required, trusted-root, and
snapshot metadata fields. The update flow is intentionally stateful:

```text
idle -> refreshing -> available/downloading -> verifying -> ready -> applying
                                              -> failed
                                              -> rolled-back
```

`lib/Terra/system/update-client/update-daemon/update-daemon.c` now uses
Terrarium APIs instead of host `curl`, host files, and an infinite sleep loop.
It performs a bounded policy/repository/update check and reports when the
service timer/event-loop backend is missing.

`updatectl` exposes the same boundary for manual diagnostics:

- `updatectl status`
- `updatectl refresh`
- `updatectl check`
- `updatectl stage <snapshot>`
- `updatectl apply`
- `updatectl rollback <snapshot>`

## Signing policy

The signing API models:

- package, repository, service, driver, and boot-image subjects
- trusted roots
- root revocation
- required channel policy
- mandatory signature and trusted-root checks

Default policy is deliberately strict: require signatures, require trusted
roots, and reject revoked roots. Policy mutation, trusted-root storage, and revocation now have hosted bootstrap backends. Real cryptographic verification and durable secure key storage are still pending.

## Recovery and safe mode

`system/recovery` defines the recovery operations expected by the boot and init
layers:

- read current recovery mode
- enter safe mode with a reason
- collect logs
- repair a filesystem
- rollback a package snapshot
- disable a service
- apply a typed recovery plan

`recoveryctl` exposes those actions for diagnostics. The recovery API now records safe-mode, log collection, filesystem repair, service disablement, and package rollback state locally. Minimal boot targets, fsck adapters, durable log export, and rollback storage are still pending.

## Audit, capabilities, sandboxing, and secrets

Capability bit helpers are implemented in `system/auth/caps.c` so user-space can
reason about capability masks consistently. Hosted audit logging, sandbox policy
application, and secrets/key records are now implemented under `system/auth`, and
`auditctl`, `sandboxctl`, and `caps` use those APIs. Kernel credential
enforcement and durable policy storage are still pending.

Secrets/key storage currently uses a hosted in-memory bootstrap store. Package
trust roots and signing policy require a future durable secure key store before
updates can be trusted.

## Crash dumps and diagnostics

The crash and developer diagnostics APIs now have hosted implementations:

- `terra_crash_write_report`
- `terra_debug_print` / `terra_debug_printf`
- `terra_symbols_load` / `terra_symbols_lookup`
- `terra_backtrace_capture`

These now provide hosted crash-report ring storage, symbol registration/loading, and deterministic backtrace fallback behavior. Real dump persistence, symbol indexes, and process backtrace capture still need kernel/debug service support.



