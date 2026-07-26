# ADR 0002: Package and Update Model

Status: accepted

## Context

TerrariumOS needs a maintainable post-install lifecycle before applications and
services can be treated as durable system components. Updates must be
transactional, signed, and recoverable.

## Decision

Terrarium packages use the `TPKG` package boundary and versioned manifests.
Package mutation is split into solve, prepare, commit, and rollback phases.
Repositories publish signed snapshot metadata, and updates move through an
explicit state machine from refresh to verification to apply or rollback.

## Consequences

- Install tools must not mutate the filesystem directly without a transaction.
- Update clients must reject unsigned or untrusted metadata once crypto/key
  storage exists.
- Recovery mode must be able to roll back named package snapshots.
- Early hosted stubs may return `TERRA_STATUS_NOTREADY`, but they must preserve
  the final transaction shape.
