#include "../common.h"

int main(int argc, char **argv) {
    if (util_is_help(argc, argv) || argc < 3) {
        util_usage("drvunbind", "<driver> <device>");
        return argc < 3 ? 1 : 0;
    }
    return util_backend_missing("drvunbind", "/run/devices/unbind", "driver unbinding endpoint is not available");
}
