#include "../common.h"

#include <stdio.h>

int main(int argc, char *argv[]) {
    if (util_is_help(argc, argv)) {
        util_usage("lspci", "");
        return 0;
    }

    puts("PCI device enumeration requires the Terrarium PCI service API.");
    return 0;
}
