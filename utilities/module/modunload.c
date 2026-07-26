#include "../common.h"

#include <stdio.h>

int main(int argc, char *argv[]) {
    if (util_is_help(argc, argv) || argc < 2) {
        util_usage("modunload", "<module>");
        return argc < 2 ? 1 : 0;
    }

    return util_backend_missing("modunload", "/proc/modules/unload", "module unloading syscall is not exposed yet");
}
