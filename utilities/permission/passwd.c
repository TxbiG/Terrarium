#include "../common.h"

int main(int argc, char *argv[]) {
    if (util_is_help(argc, argv)) {
        util_usage("passwd", "[user]");
        return 0;
    }

    return util_backend_missing("passwd", "/run/security/passwd", "password management service is not available");
}
