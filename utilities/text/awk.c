#include "../common.h"

int main(int argc, char **argv) {
    if (util_is_help(argc, argv)) { util_usage("awk", "PROGRAM [FILE]..."); return 0; }
    if (argc < 2) { util_usage("awk", "PROGRAM [FILE]..."); return 1; }

    fprintf(stderr, "awk: interpreter backend not yet implemented\n");
    return 2;
}
