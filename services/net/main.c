#include "main.h"
#include "common/service_runtime.h"
#include "netmgr/config.h"

#include <stdio.h>
#include <string.h>

typedef struct net_profile_state {
    const char *name;
    const char *method;
    const char *address;
    const char *gateway;
    int enabled;
} net_profile_state_t;

static int net_ready;
static terrarium_net_link_state_t link_state;
static net_profile_state_t profiles[] = {
    {"eth0", "dhcp", "", "", 1},
    {"eth1", "static", "192.168.1.100/24", "192.168.1.1", 1},
};

int net_service_start(void) {
    net_ready = 1;
    link_state = TERRARIUM_NET_LINK_DOWN;
    return 0;
}

int net_service_status(void) {
    return net_ready;
}

terrarium_net_link_state_t net_service_link_state(void) {
    return link_state;
}

void net_service_set_link_state(terrarium_net_link_state_t state) {
    link_state = state;
}

static void print_profiles(void) {
    printf("profiles from %s\n", TERRARIUM_NET_CONFIG_PATH);
    for (unsigned i = 0; i < sizeof(profiles) / sizeof(profiles[0]); ++i) {
        printf("  %-6s method=%-6s enabled=%s", profiles[i].name, profiles[i].method, profiles[i].enabled ? "yes" : "no");
        if (profiles[i].address && profiles[i].address[0])
            printf(" address=%s", profiles[i].address);
        if (profiles[i].gateway && profiles[i].gateway[0])
            printf(" gateway=%s", profiles[i].gateway);
        printf("\n");
    }
}

static void print_status(void) {
    printf("net: %s link=%s profiles=%u\n",
           net_service_status() ? "running" : "stopped",
           net_service_link_state() == TERRARIUM_NET_LINK_UP ? "up" : "down",
           (unsigned)(sizeof(profiles) / sizeof(profiles[0])));
}

int main(int argc, char *argv[]) {
    if (argc > 1 && strcmp(argv[1], "--status") == 0) {
        print_status();
        return net_service_status() ? 0 : 1;
    }

    if (argc > 1 && strcmp(argv[1], "--profiles") == 0) {
        net_service_start();
        print_profiles();
        return 0;
    }

    return terrarium_service_main("net", argc, argv, net_service_start, net_service_status);
}
