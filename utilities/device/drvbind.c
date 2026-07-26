#include "../common.h"

int main(int argc, char **argv) {
    if (util_is_help(argc, argv) || argc < 3) {
        util_usage("drvbind", "<driver> <device>");
        return argc < 3 ? 1 : 0;
    }
    return util_backend_missing("drvbind", "/run/devices/bind", "driver binding endpoint is not available");
}
