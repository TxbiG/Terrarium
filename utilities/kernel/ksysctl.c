#include "../common.h"

#include <stdio.h>

int main(int argc, char *argv[]) {
    if (util_is_help(argc, argv)) {
        util_usage("ksysctl", "[key[=value]]");
        return 0;
    }

    if (argc == 1)
        return util_print_file("ksysctl", "/proc/sys/kernel");

    if (strchr(argv[1], '='))
        return util_backend_missing("ksysctl", "/proc/sys/kernel", "write support is not exposed yet");

    char path[256];
    snprintf(path, sizeof(path), "/proc/sys/kernel/%s", argv[1]);
    return util_print_file("ksysctl", path);
}
