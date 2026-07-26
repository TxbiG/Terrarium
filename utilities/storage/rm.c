#include "../common.h"

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    int status = 0;

    if (util_is_help(argc, argv)) {
        util_usage("rm", "<file>...");
        return 0;
    }

    if (argc < 2) {
        util_usage("rm", "<file>...");
        return 1;
    }

    for (int i = 1; i < argc; ++i) {
        if (unlink(argv[i]) != 0) {
            fprintf(stderr, "rm: %s: %s\n", argv[i], strerror(errno));
            status = 1;
        }
    }

    return status;
}
