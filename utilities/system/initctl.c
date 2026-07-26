#include "../common.h"

int main(int argc, char **argv) {
    if (util_is_help(argc, argv)) {
        util_usage("initctl", "[status|list|start|stop|restart] [unit]");
        return 0;
    }
    return util_backend_missing("initctl", "/run/services/control", "init/service manager control endpoint is not available");
}
