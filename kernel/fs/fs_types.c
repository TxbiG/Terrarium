#include "fs_types.h"

void terra_fs_register_builtin_types(void) {
    terra_fs_register_type("tmpfs", TERRA_FS_STATUS_READWRITE, TERRA_FS_TYPE_PSEUDO | TERRA_FS_TYPE_MOUNTABLE);
    terra_fs_register_type("devfs", TERRA_FS_STATUS_READWRITE, TERRA_FS_TYPE_PSEUDO | TERRA_FS_TYPE_MOUNTABLE);
    terra_fs_register_type("procfs", TERRA_FS_STATUS_READONLY, TERRA_FS_TYPE_PSEUDO | TERRA_FS_TYPE_MOUNTABLE);
    terra_fs_register_type("initrd", TERRA_FS_STATUS_READONLY, TERRA_FS_TYPE_PSEUDO | TERRA_FS_TYPE_MOUNTABLE);
    terra_fs_register_type("fat32", TERRA_FS_STATUS_READONLY, TERRA_FS_TYPE_DISK | TERRA_FS_TYPE_MOUNTABLE);
    terra_fs_register_type("ext2", TERRA_FS_STATUS_READONLY, TERRA_FS_TYPE_DISK | TERRA_FS_TYPE_MOUNTABLE);
    terra_fs_register_type("exfat", TERRA_FS_STATUS_READONLY, TERRA_FS_TYPE_DISK | TERRA_FS_TYPE_MOUNTABLE);
    terra_fs_register_type("ext4", TERRA_FS_STATUS_READONLY, TERRA_FS_TYPE_DISK | TERRA_FS_TYPE_MOUNTABLE);
    terra_fs_register_type("ntfs3", TERRA_FS_STATUS_READONLY, TERRA_FS_TYPE_DISK | TERRA_FS_TYPE_MOUNTABLE);
    terra_fs_register_type("xfs", TERRA_FS_STATUS_READONLY, TERRA_FS_TYPE_DISK | TERRA_FS_TYPE_MOUNTABLE);
}
