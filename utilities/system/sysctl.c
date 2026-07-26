#include "../common.h"

#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
    char path[256];

    if (util_is_help(argc, argv)) {
        util_usage("sysctl", "[key[=value]]");
        return 0;
    }

    if (argc == 1)
        return util_print_file("sysctl", "/proc/sys");

    if (strchr(argv[1], '='))
        return util_backend_missing("sysctl", "/proc/sys", "runtime sysctl writes are not exposed yet");

    snprintf(path, sizeof(path), "/proc/sys/%s", argv[1]);
    return util_print_file("sysctl", path);
}
