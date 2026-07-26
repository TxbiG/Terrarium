#include "../common.h"

int main(int argc, char *argv[]) {
    if (util_is_help(argc, argv)) {
        util_usage("su", "[user]");
        return 0;
    }

    return util_backend_missing("su", "/run/security/su", "privilege transition service is not available");
}
