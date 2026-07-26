# ADR 0003: Early Security Posture

Status: accepted

## Context

The project is still early, but update, driver, service, and package security
contracts need to be strict enough that unsafe defaults do not become permanent.

## Decision

TerrariumOS will prefer strict failure for signing and privileged policy paths.
Default signing policy requires signatures, trusted roots, and revocation checks.
Missing crypto, audit, sandbox, or secrets backends should return explicit
backend errors rather than silently allowing privileged work.

## Consequences

- Developer builds may need explicit future opt-in flags for permissive mode.
- Package repositories that disable TLS or signatures are rejected by policy.
- `auditctl`, `sandboxctl`, and `caps` should not invent local host fallbacks.
- Secure key storage is a blocker before updates are considered trusted.
