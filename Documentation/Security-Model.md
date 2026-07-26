# Security Model

Terrarium security is built around explicit capabilities, signing policy,
auditable decisions, and service-owned privileged operations.

## Capabilities

`system/auth/caps.h` defines user-space capability bit helpers. These are pure
helpers for inspecting, granting, and dropping bits; real process credentials
still need kernel integration.

## Signing policy

Phase 10 defines signing subjects for:

- packages
- repositories
- services
- drivers
- boot images

Default policy requires signatures, trusted roots, and revocation checks.
Policy storage, cryptographic verification, and secure key storage are pending.

## Audit and sandboxing

`auditctl`, `sandboxctl`, and `caps` intentionally fail through explicit backend
messages until these service/kernel endpoints exist:

- `/run/security/audit`
- `/run/security/sandbox`
- `/run/security/caps`

Do not add local host fallbacks for these commands; the security model must be
owned by Terrarium kernel/service contracts.

## Secrets and keys

Trust roots and update keys must eventually live in a dedicated secure key store
with revocation support. Early builds should prefer strict failure over accepting
unsigned or untrusted update content.
