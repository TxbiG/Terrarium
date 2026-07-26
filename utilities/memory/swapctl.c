#include "../common.h"

int main(int argc, char **argv) {
    if (util_is_help(argc, argv)) {
        util_usage("swapctl", "[status|on|off]");
        return 0;
    }
    return util_backend_missing("swapctl", "/proc/swaps", "swap control syscall is not exposed yet");
}
