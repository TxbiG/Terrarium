#include "../common.h"

#include <stdio.h>

int main(int argc, char *argv[]) {
    if (util_is_help(argc, argv)) {
        util_usage("ksysinfo", "");
        return 0;
    }

    puts("TerrariumOS kernel");
    puts("arch: configured at boot");
    puts("drivers: pci serial virtio");
    return 0;
}
