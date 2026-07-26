#include "../common.h"

int main(int argc, char *argv[]) {
    if (util_is_help(argc, argv) || argc < 2) {
        util_usage("umount", "<directory>");
        return argc < 2 ? 1 : 0;
    }

    return util_backend_missing("umount", "/proc/mounts", "unmount syscall is not exposed yet");
}
