#include "../common.h"

int main(int argc, char *argv[]) {
    if (util_is_help(argc, argv)) {
        util_usage("ipcs", "");
        return 0;
    }

    return util_print_file("ipcs", "/proc/ipc");
}
