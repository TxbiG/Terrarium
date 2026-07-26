# Recovery and Safe Mode

Recovery is the path for repairing a Terrarium installation without requiring a
fully healthy desktop session.

## Recovery operations

The `system/recovery` API models:

- read current recovery mode
- enter safe mode with a reason
- collect logs
- repair a filesystem
- rollback a package snapshot
- disable a service
- apply a typed recovery plan

`recoveryctl` exposes those actions for diagnostics.

## Safe mode target

A minimal safe-mode boot should eventually start only:

1. kernel logging
2. root filesystem read/write or repair mount
3. init/service manager in recovery profile
4. shell/console
5. package rollback and service disablement tools
6. log export tools

Lumina, Nova, networking, and nonessential services should be opt-in from safe
mode until they have health checks.

## Pending backend work

- boot-loader recovery target selection
- init recovery profile
- filesystem repair adapters
- crash/log bundle export
- package snapshot storage
- service disablement persistence
