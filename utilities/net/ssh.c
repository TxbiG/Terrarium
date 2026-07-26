#include "../common.h"

int main(int argc, char *argv[]) {
    if (util_is_help(argc, argv) || argc < 2) {
        util_usage("ssh", "<host>");
        return argc < 2 ? 1 : 0;
    }

    return util_backend_missing("ssh", "/run/net/ssh", "SSH client service is not available");
}
