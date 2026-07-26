#include "../common.h"

#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    const char *home;
    char path[512];

    if (util_is_help(argc, argv)) {
        util_usage("history", "");
        return 0;
    }

    home = getenv("HOME");
    if (!home || !home[0])
        home = ".";

    snprintf(path, sizeof(path), "%s/.terrarium_history", home);
    return util_print_file("history", path);
}
