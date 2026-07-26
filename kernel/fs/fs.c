#include "blockdev.h"
#include "fs.h"
#include "fs_types.h"
#include "vfs_core.h"

static int fs_ready;

void fs_init(const terra_boot_info_t *boot_info) {
    (void)boot_info;
    terra_blockdev_reset();
    terra_vfs_reset();
    terra_fs_register_builtin_types();
    fs_ready = terra_vfs_bootstrap() == 0;
}

int fs_is_ready(void) {
    return fs_ready;
}

void fs_shutdown(void) {
    terra_vfs_reset();
    terra_blockdev_reset();
    fs_ready = 0;
}
