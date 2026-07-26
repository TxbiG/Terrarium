#include "../common.h"

#include <stdio.h>

int main(int argc, char *argv[]) {
    if (util_is_help(argc, argv) || argc < 2) {
        util_usage("modload", "<module-path>");
        return argc < 2 ? 1 : 0;
    }

    return util_backend_missing("modload", "/proc/modules/load", "module loading syscall is not exposed yet");
}
