#include "../common.h"

int main(int argc, char **argv) {
    if (util_is_help(argc, argv)) { util_usage("nice", "[-n PRIORITY] COMMAND [ARG]..."); return 0; }
    fprintf(stderr, "nice: scheduler priority API required\n");
    return util_backend_missing("nice", "/proc/process", "scheduler priority API required");
}
