#include "../common.h"

int main(int argc, char *argv[]) {
    if (util_is_help(argc, argv)) {
        util_usage("reboot", "");
        return 0;
    }

    return util_backend_missing("reboot", "/proc/sysrq-trigger", "kernel reboot syscall is not exposed yet");
}
