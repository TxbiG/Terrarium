# Package Management API

Terrarium package management is defined by the public `system/package` API.
The implementation currently validates contracts and reports missing backends
with Terrarium status codes while the install database, archive unpacker,
dependency database, crypto verifier, and rollback store are built.

## Package format

- Magic: `TPKG`
- Format version: `TERRA_PACKAGE_FORMAT_VERSION`
- Manifest: identity, architecture, kind, description, dependencies, content
  hash, signature identity, and rollback group.

## Repository metadata

Repositories must declare:

- name and URL
- channel: stable, beta, or dev
- trusted root identifier
- TLS requirement
- signature requirement
- snapshot metadata and repository signature

A repository that disables TLS or signatures should be rejected by policy for
normal update channels.

## Install transactions

Package mutation is split into explicit phases:

1. solve dependencies/conflicts
2. prepare transaction
3. commit transaction
4. rollback transaction if validation fails

This separation is required for crash-safe updates and recovery-mode rollback.

## Update flow

The update state machine is:

```text
idle -> refreshing -> available/downloading -> verifying -> ready -> applying
                                              -> failed
                                              -> rolled-back
```

`updatectl` and the update daemon both use this API surface.
