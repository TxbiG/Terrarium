#include "../common.h"

#include <auth/caps.h>
#include <stdlib.h>

static enum terra_capability parse_cap(const char *name) {
    if (strcmp(name, "sys_admin") == 0) return TERRA_CAP_SYS_ADMIN;
    if (strcmp(name, "net_admin") == 0) return TERRA_CAP_NET_ADMIN;
    if (strcmp(name, "fs_admin") == 0) return TERRA_CAP_FS_ADMIN;
    if (strcmp(name, "power") == 0) return TERRA_CAP_POWER;
    if (strcmp(name, "debug") == 0) return TERRA_CAP_DEBUG;
    return 0;
}

int main(int argc, char **argv) {
    if (util_is_help(argc, argv) || argc < 2) {
        util_usage("caps", "[list|has <mask> <cap>|grant <mask> <cap>|drop <mask> <cap>]");
        return argc < 2 ? 1 : 0;
    }
    if (strcmp(argv[1], "list") == 0) {
        puts("sys_admin net_admin fs_admin power debug");
        return 0;
    }
    if (argc > 3) {
        terra_capset_t mask = (terra_capset_t)strtoull(argv[2], 0, 0);
        enum terra_capability cap = parse_cap(argv[3]);
        if (!cap) {
            fprintf(stderr, "caps: unknown capability: %s\n", argv[3]);
            return 1;
        }
        if (strcmp(argv[1], "has") == 0) {
            printf("%s\n", terra_caps_has(mask, cap) ? "yes" : "no");
            return terra_caps_has(mask, cap) ? 0 : 2;
        }
        if (strcmp(argv[1], "grant") == 0) {
            printf("%llu\n", (unsigned long long)terra_caps_grant(mask, cap));
            return 0;
        }
        if (strcmp(argv[1], "drop") == 0) {
            printf("%llu\n", (unsigned long long)terra_caps_drop(mask, cap));
            return 0;
        }
    }
    util_usage("caps", "[list|has <mask> <cap>|grant <mask> <cap>|drop <mask> <cap>]");
    return 1;
}
