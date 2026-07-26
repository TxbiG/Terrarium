#include "../common.h"

#include <stdio.h>

int main(int argc, char *argv[]) {
    if (util_is_help(argc, argv)) {
        util_usage("alias", "[name=value]...");
        return 0;
    }

    if (argc == 1)
        return util_print_file("alias", "/run/shell/aliases");

    return util_backend_missing("alias", "/run/shell/aliases", "shell alias mutation is handled by the active shell");
}
