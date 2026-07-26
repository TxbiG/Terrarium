#include "../common.h"

#include <stdio.h>

int main(int argc, char *argv[]) {
    if (util_is_help(argc, argv)) {
        util_usage("ps", "");
        return 0;
    }

    puts("PID  STATE    COMMAND");
    puts("1    running  init");
    return 0;
}
