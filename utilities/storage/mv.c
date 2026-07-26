#include "../common.h"

#include <errno.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (util_is_help(argc, argv)) {
        util_usage("mv", "<source> <destination>");
        return 0;
    }

    if (argc != 3) {
        util_usage("mv", "<source> <destination>");
        return 1;
    }

    if (rename(argv[1], argv[2]) != 0) {
        fprintf(stderr, "mv: %s -> %s: %s\n", argv[1], argv[2], strerror(errno));
        return 1;
    }

    return 0;
}
