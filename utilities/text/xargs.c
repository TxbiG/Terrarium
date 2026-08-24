#include "../common.h"

#include <stdlib.h>

int main(int argc, char **argv) {
    if (util_is_help(argc, argv)) { util_usage("xargs", "COMMAND [ARG]..."); return 0; }
    if (argc < 2) { util_usage("xargs", "COMMAND [ARG]..."); return 1; }

    fprintf(stderr, "xargs: execution backend is not yet connected to the Terrarium process API\n");
    return util_backend_missing("xargs", "/proc/process", "spawn/exec interface required");
}
