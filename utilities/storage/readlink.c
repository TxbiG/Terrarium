#include "../common.h"

#include <unistd.h>

int main(int argc, char **argv) {
    char buf[4096];
    if (util_is_help(argc, argv)) { util_usage("readlink", "LINK..."); return 0; }
    if (argc < 2) { util_usage("readlink", "LINK..."); return 1; }

    int status = 0;
    for (int i = 1; i < argc; ++i) {
        ssize_t n = readlink(argv[i], buf, sizeof(buf) - 1);
        if (n < 0) {
            fprintf(stderr, "readlink: %s: %s\n", argv[i], strerror(errno));
            status = 1;
            continue;
        }
        buf[n] = '\0';
        puts(buf);
    }
    return status;
}
