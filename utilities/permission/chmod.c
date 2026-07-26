#include "../common.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

int main(int argc, char *argv[]) {
    if (util_is_help(argc, argv)) {
        util_usage("chmod", "<octal-mode> <file>...");
        return 0;
    }

    if (argc < 3) {
        util_usage("chmod", "<octal-mode> <file>...");
        return 1;
    }

    mode_t mode = (mode_t)strtol(argv[1], 0, 8);
    int status = 0;
    for (int i = 2; i < argc; ++i) {
        if (chmod(argv[i], mode) != 0) {
            fprintf(stderr, "chmod: %s: %s\n", argv[i], strerror(errno));
            status = 1;
        }
    }
    return status;
}
