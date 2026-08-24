#include "../common.h"

int main(int argc, char **argv) {
    if (util_is_help(argc, argv)) { util_usage("useradd", "USER"); return 0; }
    fprintf(stderr, "useradd: Terrarium identity backend is not connected yet\n");
    return util_backend_missing("useradd", "/etc/passwd", "account database and identity-management API required");
}
