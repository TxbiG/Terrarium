#include "../common.h"

int main(int argc, char **argv) {
    if (util_is_help(argc, argv)) { util_usage("pmap", "PID"); return 0; }
    fprintf(stderr, "pmap: backend is not connected yet\n");
    return util_backend_missing("pmap", "/proc/PID/maps", "process memory-map API required");
}
