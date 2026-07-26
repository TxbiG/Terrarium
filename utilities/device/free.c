#include "../common.h"

#include <stdio.h>

int main(int argc, char *argv[]) {
    if (util_is_help(argc, argv)) {
        util_usage("free", "");
        return 0;
    }

    puts("              total        used        free");
    puts("Mem:            n/a         n/a         n/a");
    puts("Swap:           n/a         n/a         n/a");
    return 0;
}
