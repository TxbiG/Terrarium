#include "../common.h"

#include <unistd.h>

int main(int argc, char **argv) {
    char name[256];
    if (util_is_help(argc, argv)) { util_usage("hostname", "[NAME]"); return 0; }

    if (argc == 1) {
        if (gethostname(name, sizeof(name)) != 0) {
            return util_backend_missing("hostname", "/proc/sys/kernel/hostname", strerror(errno));
        }
        name[sizeof(name) - 1] = '\0';
        puts(name);
        return 0;
    }

    if (argc == 2) {
        fprintf(stderr, "hostname: Terrarium hostname setter is not connected yet\n");
        return util_backend_missing("hostname", "/proc/sys/kernel/hostname", "hostname setter required");
    }

    util_usage("hostname", "[NAME]");
    return 1;
}
