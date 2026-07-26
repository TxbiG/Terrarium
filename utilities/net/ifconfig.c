#include "../common.h"
#include <net/net.h>

static int print_interfaces_from_api(void) {
    terra_net_interface_t interfaces[16];
    int count = terra_net_list_interfaces(interfaces, 16);
    if (count <= 0)
        return -1;

    for (int i = 0; i < count; ++i) {
        printf("%s: state=%s rx=%llu tx=%llu\n",
               interfaces[i].name,
               interfaces[i].state == TERRA_NET_STATE_UP ? "up" : "down",
               (unsigned long long)interfaces[i].rx_bytes,
               (unsigned long long)interfaces[i].tx_bytes);
    }
    return 0;
}

int main(int argc, char *argv[]) {
    if (util_is_help(argc, argv)) {
        util_usage("ifconfig", "");
        return 0;
    }

    if (print_interfaces_from_api() == 0)
        return 0;
    return util_print_file("ifconfig", "/proc/net/dev");
}
