#include "../common.h"

int main(int argc, char **argv) {
    if (util_is_help(argc, argv)) { util_usage("traceroute", "HOST"); return 0; }
    if (argc != 2) { util_usage("traceroute", "HOST"); return 1; }
    fprintf(stderr, "traceroute: raw socket/TTL networking backend is not connected yet\n");
    return util_backend_missing("traceroute", "/proc/net", "raw socket and TTL API required");
}
