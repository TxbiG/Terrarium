#include "../common.h"

int main(int argc, char **argv) {
    if (util_is_help(argc, argv)) { util_usage("schedstat", ""); return 0; }
    fprintf(stderr, "schedstat: scheduler statistics API required\n");
    return util_backend_missing("schedstat", "/proc/process", "scheduler statistics API required");
}
