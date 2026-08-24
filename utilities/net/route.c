#include "../common.h"

int main(int argc, char **argv) {
    if (util_is_help(argc, argv)) { util_usage("route", "[show|add|del] ..."); return 0; }
    fprintf(stderr, "route: Terrarium routing backend is not connected yet\n");
    return util_backend_missing("route", "/proc/net/route", "routing API required");
}
