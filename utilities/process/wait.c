#include "../common.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    long pid;

    if (util_is_help(argc, argv) || argc < 2) {
        util_usage("wait", "<pid>");
        return argc < 2 ? 1 : 0;
    }

    pid = strtol(argv[1], 0, 10);
    if (pid <= 0) {
        fprintf(stderr, "wait: invalid pid '%s'\n", argv[1]);
        return 1;
    }

    return util_backend_missing("wait", "/proc/wait", "process wait syscall is not exposed to this utility yet");
}
