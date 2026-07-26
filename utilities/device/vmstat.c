#include "../common.h"

#include <stdio.h>

int main(int argc, char *argv[]) {
    if (util_is_help(argc, argv)) {
        util_usage("vmstat", "");
        return 0;
    }

    puts("procs -----------memory---------- ---swap-- -----io---- -system-- ------cpu-----");
    puts(" r  b   free   cache   si   so    bi    bo   in   cs us sy id");
    puts(" 0  0    n/a     n/a    0    0     0     0    0    0  0  0 100");
    return 0;
}
