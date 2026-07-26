#include "../common.h"

int main(int argc, char **argv) {
    if (util_is_help(argc, argv)) {
        util_usage("servicectl", "[status|list|start|stop|restart] [service]");
        return 0;
    }
    return util_backend_missing("servicectl", "/run/services/control", "service manager control endpoint is not available");
}
