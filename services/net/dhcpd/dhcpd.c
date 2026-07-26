#include "common/service_runtime.h"
#include <stdio.h>
#include <string.h>

static int dhcpd_ready;
static const char *dhcp_mode = "client";

int terrarium_dhcpd_start(void) {
    dhcpd_ready = 1;
    return 0;
}

int terrarium_dhcpd_status(void) {
    return dhcpd_ready;
}

static void print_status(void) {
    printf("dhcpd: %s mode=%s leases=%u\n", dhcpd_ready ? "running" : "stopped", dhcp_mode, 0u);
}

int main(int argc, char **argv) {
    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "--server") == 0)
            dhcp_mode = "server";
        else if (strcmp(argv[i], "--client") == 0)
            dhcp_mode = "client";
        else if (strcmp(argv[i], "--status") == 0) {
            print_status();
            return dhcpd_ready ? 0 : 1;
        }
    }

    return terrarium_service_main("dhcpd", argc, argv, terrarium_dhcpd_start, terrarium_dhcpd_status);
}
