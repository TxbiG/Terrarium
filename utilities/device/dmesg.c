#include "../common.h"

int main(int argc, char *argv[]) {
    if (util_is_help(argc, argv)) {
        util_usage("dmesg", "");
        return 0;
    }

    return util_print_file("dmesg", "/proc/kmsg");
}
