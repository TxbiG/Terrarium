#include "../common.h"

#include <stdio.h>

int main(int argc, char *argv[]) {
    if (util_is_help(argc, argv)) {
        util_usage("terrarium", "[about|version|status]");
        return 0;
    }

    const char *command = argc > 1 ? argv[1] : "about";
    if (strcmp(command, "about") == 0 || strcmp(command, "version") == 0) {
        puts("TerrariumOS");
        puts("Userland utility shell");
        return 0;
    }

    if (strcmp(command, "status") == 0)
        return util_backend_missing("terrarium", "/run/terrarium/status", "system status service is not available");

    util_usage("terrarium", "[about|version|status]");
    return 1;
}
