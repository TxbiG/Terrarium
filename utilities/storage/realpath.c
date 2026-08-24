#include "../common.h"

#include <stdlib.h>
#include <limits.h>

int main(int argc, char **argv) {
    char resolved[PATH_MAX];
    if (util_is_help(argc, argv)) { util_usage("realpath", "PATH..."); return 0; }
    if (argc < 2) { util_usage("realpath", "PATH..."); return 1; }

    int status = 0;
    for (int i = 1; i < argc; ++i) {
        if (!realpath(argv[i], resolved)) {
            fprintf(stderr, "realpath: %s: %s\n", argv[i], strerror(errno));
            status = 1;
            continue;
        }
        puts(resolved);
    }
    return status;
}
