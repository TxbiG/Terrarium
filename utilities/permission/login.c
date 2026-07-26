#include "../common.h"

int main(int argc, char *argv[]) {
    if (util_is_help(argc, argv)) {
        util_usage("login", "[user]");
        return 0;
    }

    return util_backend_missing("login", "/run/session/login", "session authentication service is not available");
}
