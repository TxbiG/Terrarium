#include "../common.h"

int main(int argc, char **argv) {
    if (util_is_help(argc, argv)) { util_usage("proctree", ""); return 0; }
    fprintf(stderr, "proctree: process enumeration API required\n");
    return util_backend_missing("proctree", "/proc/process", "process enumeration API required");
}
