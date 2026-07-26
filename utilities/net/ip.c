#include "../common.h"

#include <net/net.h>
#include <net/route.h>
#include <stdio.h>
#include <string.h>

static int print_links(void) {
    terra_net_interface_t interfaces[16];
    int count = terra_net_list_interfaces(interfaces, 16);
    if (count <= 0)
        return -1;
    for (int i = 0; i < count; ++i)
        printf("%d: %s: state %s rx %llu tx %llu\n", i + 1, interfaces[i].name, interfaces[i].state == TERRA_NET_STATE_UP ? "UP" : "DOWN", (unsigned long long)interfaces[i].rx_bytes, (unsigned long long)interfaces[i].tx_bytes);
    return 0;
}

static int print_routes(void) {
    terra_route_t routes[16];
    int count = terra_route_list(routes, 16);
    if (count <= 0)
        return -1;
    for (int i = 0; i < count; ++i)
        printf("route %d dev %s prefix %u\n", i, routes[i].interface_name, routes[i].prefix_len);
    return 0;
}

int main(int argc, char *argv[]) {
    if (util_is_help(argc, argv)) {
        util_usage("ip", "<addr|link|route>");
        return 0;
    }

    if (argc < 2 || strcmp(argv[1], "link") == 0 || strcmp(argv[1], "addr") == 0) {
        if (print_links() == 0)
            return 0;
        return util_print_file("ip", "/proc/net/dev");
    }
    if (strcmp(argv[1], "route") == 0) {
        if (print_routes() == 0)
            return 0;
        return util_print_file("ip", "/proc/net/route");
    }

    fprintf(stderr, "ip: unknown object '%s'\n", argv[1]);
    return 1;
}
