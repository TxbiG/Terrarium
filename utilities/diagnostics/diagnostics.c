#include "../common.h"

int main(int argc, char **argv) {
    if (util_is_help(argc, argv)) {
        util_usage("diagnostics", "[summary|collect]");
        return 0;
    }
    return util_backend_missing("diagnostics", "/run/diagnostics", "diagnostic collection service is not available");
}
