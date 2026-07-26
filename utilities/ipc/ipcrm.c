#include "../common.h"

#include <stdio.h>

int main(int argc, char *argv[]) {
    if (util_is_help(argc, argv) || argc < 3) {
        util_usage("ipcrm", "<queue|shared|semaphore> <id>");
        return argc < 3 ? 1 : 0;
    }

    return util_backend_missing("ipcrm", "/proc/ipc/control", "IPC removal syscall is not exposed yet");
}
