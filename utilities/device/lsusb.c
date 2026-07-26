#include "../common.h"

#include <stdio.h>

int main(int argc, char *argv[]) {
    if (util_is_help(argc, argv)) {
        util_usage("lsusb", "");
        return 0;
    }

    puts("USB device enumeration requires the Terrarium USB service API.");
    return 0;
}
