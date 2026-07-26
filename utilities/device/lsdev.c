#include "../common.h"

#include <stdio.h>

int main(int argc, char *argv[]) {
    if (util_is_help(argc, argv)) {
        util_usage("lsdev", "");
        return 0;
    }

    puts("CLASS      DEVICE      DRIVER");
    puts("bus        pci         pci");
    puts("bus        serial      serial");
    puts("bus        virtio      virtio");
    return 0;
}
