#include "../common.h"

#include <stdio.h>
#include <time.h>

int main(int argc, char *argv[]) {
    if (util_is_help(argc, argv)) {
        util_usage("uptime", "");
        return 0;
    }

    printf("uptime: %ld seconds since command runtime started\n", (long)time(NULL));
    return 0;
}
