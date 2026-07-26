#include "../common.h"

#include <stdio.h>

int main(int argc, char *argv[]) {
    if (util_is_help(argc, argv) || argc < 2) {
        util_usage("modprobe", "<module>");
        return argc < 2 ? 1 : 0;
    }

    return util_backend_missing("modprobe", "/proc/modules/load", "dependency-aware module loading is not exposed yet");
}
