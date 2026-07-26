#include "../common.h"

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

int main(int argc, char *argv[]) {
    int status = 0;

    if (util_is_help(argc, argv)) {
        util_usage("mkdir", "<directory>...");
        return 0;
    }

    if (argc < 2) {
        util_usage("mkdir", "<directory>...");
        return 1;
    }

    for (int i = 1; i < argc; ++i) {
        if (mkdir(argv[i], 0777) != 0) {
            fprintf(stderr, "mkdir: %s: %s\n", argv[i], strerror(errno));
            status = 1;
        }
    }

    return status;
}
