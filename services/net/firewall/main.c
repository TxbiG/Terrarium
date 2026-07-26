#include "common/service_runtime.h"
#include <stdio.h>
#include <string.h>

static int firewall_ready;
static const char *default_policy = "accept";

static int firewalld_start(void) {
    firewall_ready = 1;
    return 0;
}

static int firewalld_status(void) {
    return firewall_ready;
}

static void print_status(void) {
    printf("firewalld: %s default=%s rules=%u\n", firewall_ready ? "running" : "stopped", default_policy, 0u);
}

int main(int argc, char **argv) {
    if (argc > 1 && strcmp(argv[1], "--status") == 0) {
        print_status();
        return firewall_ready ? 0 : 1;
    }

    if (argc > 2 && strcmp(argv[1], "--default") == 0) {
        default_policy = argv[2];
        firewall_ready = 1;
        print_status();
        return 0;
    }

    return terrarium_service_main("firewalld", argc, argv, firewalld_start, firewalld_status);
}
