#include "../common.h"

int main(int argc, char *argv[]) {
    if (util_is_help(argc, argv)) {
        util_usage("shutdown", "");
        return 0;
    }

    return util_backend_missing("shutdown", "/run/power/shutdown", "power manager shutdown endpoint is not available");
}
