#include "../common.h"

#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    int signal_number = SIGTERM;
    int first_pid = 1;
    int status = 0;

    if (util_is_help(argc, argv)) {
        util_usage("kill", "[-signal] <pid>...");
        return 0;
    }

    if (argc > 1 && argv[1][0] == '-') {
        signal_number = atoi(argv[1] + 1);
        first_pid = 2;
    }

    if (argc <= first_pid) {
        util_usage("kill", "[-signal] <pid>...");
        return 1;
    }

    for (int i = first_pid; i < argc; ++i) {
        long pid = strtol(argv[i], 0, 10);
        if (kill((pid_t)pid, signal_number) != 0) {
            fprintf(stderr, "kill: %s: %s\n", argv[i], strerror(errno));
            status = 1;
        }
    }

    return status;
}
