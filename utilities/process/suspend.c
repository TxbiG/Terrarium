#include "../common.h"

int main(int argc, char *argv[]) {
    if (util_is_help(argc, argv)) {
        util_usage("suspend", "");
        return 0;
    }

    return util_backend_missing("suspend", "/run/power/suspend", "power manager suspend endpoint is not available");
}
