#include "../common.h"

int main(int argc, char **argv) {
    if (util_is_help(argc, argv)) {
        util_usage("route", "[show|add|del]");
        return 0;
    }
    return util_backend_missing("route", "/run/net/route", "route mutation/list syscall handlers are not implemented yet");
}
