#include "../common.h"

int main(int argc, char *argv[]) {
    if (util_is_help(argc, argv) || argc < 2) {
        util_usage("sudo", "<command> [args...]");
        return argc < 2 ? 1 : 0;
    }

    return util_backend_missing("sudo", "/run/security/sudo", "capability elevation service is not available");
}
