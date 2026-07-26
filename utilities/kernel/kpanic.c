#include "../common.h"

#include <stdio.h>

int main(int argc, char *argv[]) {
    if (util_is_help(argc, argv)) {
        util_usage("kpanic", "[panic-log-file]");
        return 0;
    }

    if (argc > 1)
        return util_print_file("kpanic", argv[1]);

    return util_print_file("kpanic", "/var/log/kernel/panic.log");
}
