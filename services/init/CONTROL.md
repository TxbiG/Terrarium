# Terrarium Service Control Protocol

This document sketches the Phase 6 control surface that will eventually live at
`/run/services/control`.

`terrarium-init` currently exposes the same operations through command-line
options so the service manager can be tested before IPC and process supervision
are complete.

## Commands

```text
LIST
STATUS <name>
START <name>
STOP <name>
RESTART <name>
START-ALL
```

## CLI mapping

```text
terrarium-init --list
terrarium-init --status
terrarium-init --start <name>
terrarium-init --stop <name>
terrarium-init --restart <name>
terrarium-init --dry-run
```

## Response model

The future IPC endpoint should return stable Terrarium status values plus a
small text payload for diagnostics:

```text
OK <message>
ERR <terra_status> <message>
```

## Remaining implementation

- create `/run/services/control` once tmpfs/devfs/procfs and IPC are available,
- connect the endpoint to `terra_service_*` system wrappers,
- persist service state across control requests,
- replace dry state transitions with supervised `terra_process_spawn` children,
- report child exit status and restart decisions.
## Kernel syscall ABI

The Phase 6 user-space service API currently routes through syscall IDs
`800..803`:

- `TERRA_SYSCALL_SERVICE_START`
- `TERRA_SYSCALL_SERVICE_STOP`
- `TERRA_SYSCALL_SERVICE_STATUS`
- `TERRA_SYSCALL_SERVICE_LIST`

The kernel registers these IDs as known service syscalls and returns
`TERRA_STATUS_NOTREADY` until the real init IPC bridge is available.