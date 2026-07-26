#include "../common.h"

#include <net/ip.h>
#include <net/ping.h>

int main(int argc, char *argv[]) {
    terra_ip_addr_t addr;
    terra_ping_result_t result;

    if (util_is_help(argc, argv) || argc < 2) {
        util_usage("ping", "<host>");
        return argc < 2 ? 1 : 0;
    }

    if (terra_ip_parse(argv[1], &addr) != 0)
        return util_backend_missing("ping", "/run/net/dns", "DNS resolver is not available for hostnames yet");

    if (terra_ping(&addr, &result) == 0) {
        printf("%s reachable=%s rtt=%ums ttl=%u\n", argv[1], result.reachable ? "yes" : "no", result.round_trip_ms, result.ttl);
        return result.reachable ? 0 : 1;
    }

    return util_backend_missing("ping", "/dev/icmp", "raw ICMP socket support is not exposed yet");
}
