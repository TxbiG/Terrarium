#include "../common.h"

int main(int argc, char **argv) {
    if (util_is_help(argc, argv)) {
        util_usage("hotplug", "[monitor|rescan]");
        return 0;
    }
    return util_backend_missing("hotplug", "/run/devices/hotplug", "hotplug monitor/rescan endpoint is not available");
}
