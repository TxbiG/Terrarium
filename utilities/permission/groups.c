#include "../common.h"

int main(int argc, char **argv) {
    if (util_is_help(argc, argv)) { util_usage("groups", "[USER]"); return 0; }
    fprintf(stderr, "groups: Terrarium identity backend is not connected yet\n");
    return util_backend_missing("groups", "/etc/passwd", "account/group database API required");
}
