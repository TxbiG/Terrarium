#include "../common.h"

#include <stdio.h>

int main(int argc, char *argv[]) {
    if (util_is_help(argc, argv) || argc < 3) {
        util_usage("msgsend", "<target> <message>");
        return argc < 3 ? 1 : 0;
    }

    return util_backend_missing("msgsend", "/run/ipc", "message-send service endpoint is not available");
}
