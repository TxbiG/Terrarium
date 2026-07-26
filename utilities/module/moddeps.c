#include "../common.h"

#include <stdio.h>

int main(int argc, char *argv[]) {
    char path[256];

    if (util_is_help(argc, argv) || argc < 2) {
        util_usage("moddeps", "<module>");
        return argc < 2 ? 1 : 0;
    }

    snprintf(path, sizeof(path), "/modules/%s.deps", argv[1]);
    return util_print_file("moddeps", path);
}
