#include "../common.h"

int main(int argc, char **argv) {
    if (util_is_help(argc, argv)) { util_usage("renice", "PRIORITY PID..."); return 0; }
    fprintf(stderr, "renice: scheduler priority API required\n");
    return util_backend_missing("renice", "/proc/process", "scheduler priority API required");
}
