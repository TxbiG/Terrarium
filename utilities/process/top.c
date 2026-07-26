#include "../common.h"

#include <stdio.h>

int main(int argc, char *argv[]) {
    if (util_is_help(argc, argv)) {
        util_usage("top", "");
        return 0;
    }

    puts("TerrariumOS task monitor");
    puts("PID  CPU%  MEM%  COMMAND");
    puts("1    0.0   n/a   init");
    return 0;
}
