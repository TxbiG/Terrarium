#include "../common.h"

#include <net/route.h>
#include <string.h>

static int print_routes_from_api(void) {
    terra_route_t routes[32];
    int count = terra_route_list(routes, 32);
    if (count <= 0)
        return -1;
    puts("Kernel IP routing table");
    for (int i = 0; i < count; ++i)
        printf("%-8s prefix=%u\n", routes[i].interface_name, routes[i].prefix_len);
    return 0;
}

int main(int argc, char *argv[]) {
    if (util_is_help(argc, argv)) {
        util_usage("netstat", "[-r]");
        return 0;
    }

    if (argc > 1 && strcmp(argv[1], "-r") == 0) {
        if (print_routes_from_api() == 0)
            return 0;
        return util_print_file("netstat", "/proc/net/route");
    }

    return util_print_file("netstat", "/proc/net/sockstat");
}
