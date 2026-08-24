#include "../common.h"

int main(int argc, char **argv) {
    if (util_is_help(argc, argv)) { util_usage("kmemstat", ""); return 0; }
    fprintf(stderr, "kmemstat: backend is not connected yet\n");
    return util_backend_missing("kmemstat", "/proc/meminfo", "kernel memory statistics endpoint required");
}
