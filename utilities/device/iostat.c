#include "../common.h"

#include <stdio.h>

int main(int argc, char *argv[]) {
    if (util_is_help(argc, argv)) {
        util_usage("iostat", "");
        return 0;
    }

    puts("Device            read/s    write/s");
    puts("storage0             n/a        n/a");
    return 0;
}
