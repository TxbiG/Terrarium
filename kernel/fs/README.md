# Terrarium Kernel Filesystems

The active filesystem path is the Terrarium-native VFS:

- `fs.c`: lifecycle entry point.
- `vfs_core.c`: mount table, nodes, file handles, path operations, and default `/dev` and `/proc` nodes.
- `blockdev.c`: block-device registry used by disk filesystems.
- `diskfs.c`: disk filesystem signature probes and native disk-driver registry.
- `exfat/`: native read-only exFAT boot-sector parser and volume validator.
- `ext4/`: native read-only ext4 superblock parser and feature validator.
- `ntfs3/`: native read-only NTFS boot-sector parser and volume validator.
- `xfs/`: native read-only XFS superblock parser and volume validator.
- `fs_types.c`: supported filesystem type registry.

Current status:

- `tmpfs`, `devfs`, `procfs`, and `initrd` are native Terrarium filesystem types.
- `fat32` and `ext2` are recognized disk filesystem formats.
- `exfat`, `ext4`, `ntfs3`, and `xfs` have native read-only mount validators. When mounted through `terra_vfs_mount_block`, the VFS stores parsed volume context and exposes a `.volume` metadata file at the mount point.
Write support must remain disabled until each format has real metadata
allocation, checksum, journal/log replay, rollback, and crash-consistency code.
