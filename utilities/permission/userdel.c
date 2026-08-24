#include "../common.h"

int main(int argc, char **argv) {
    if (util_is_help(argc, argv)) { util_usage("userdel", "USER"); return 0; }
    fprintf(stderr, "userdel: Terrarium identity backend is not connected yet\n");
    return util_backend_missing("userdel", "/etc/passwd", "account database and identity-management API required");
}
