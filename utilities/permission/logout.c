#include "../common.h"

int main(int argc, char *argv[]) {
    if (util_is_help(argc, argv)) {
        util_usage("logout", "");
        return 0;
    }

    return util_backend_missing("logout", "active shell session", "logout must be handled by the login shell");
}
