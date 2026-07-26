# Terrarium Init

`terrarium-init` is the Phase 6 service-manager boundary.

The current implementation loads `units/manifest.txt`, parses the listed
`units/*.service` files, and falls back to the compiled table in
`src/service_manager.c` if no manifest is available. The files are deliberately
small `key=value` records so init ownership stays clear while the runtime grows.

Supported keys:

- `name`: stable service name.
- `exec`: executable path in the installed system image.
- `description`: human-readable status text.
- `args`: default arguments passed by the service manager.
- `after`: comma-separated dependencies.
- `restart`: `never`, `on-failure`, or `always`.
- `required`: `true` for boot-critical units.

Current runtime behavior:

- `terrarium-init --units services/init/units/manifest.txt --dry-run` prints dependency-ordered startup from unit files.
- `terrarium-init --dry-run` uses `/etc/terrarium/services/manifest.txt` and falls back to compiled defaults if needed.
- `terrarium-init --start NAME --dry-run` prints the selected unit and its dependencies.
- `terrarium-init --status` prints known units and current in-memory state.
- `terrarium-init --list` prints unit names.
- `terrarium-init --stop NAME --dry-run` and `--restart NAME --dry-run` exercise the control paths without spawning or killing processes.

See `CONTROL.md` for the planned `/run/services/control` command surface.

Remaining work:

- spawn real processes through `terra_process_spawn`,
- supervise exit status and restart policy,
- expose `/run/services/control`,
- persist status for service-control utilities.