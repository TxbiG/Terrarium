#include "../common.h"

int main(int argc, char *argv[]) {
    if (util_is_help(argc, argv)) {
        util_usage("mount", "[device directory]");
        return 0;
    }

    if (argc == 1)
        return util_print_file("mount", "/proc/mounts");

    if (argc < 3) {
        util_usage("mount", "[device directory]");
        return 1;
    }

    return util_backend_missing("mount", "/proc/mounts", "mount syscall is not exposed yet");
}
