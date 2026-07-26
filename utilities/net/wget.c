#include "../common.h"

int main(int argc, char *argv[]) {
    if (util_is_help(argc, argv) || argc < 2) {
        util_usage("wget", "<url>");
        return argc < 2 ? 1 : 0;
    }

    return util_backend_missing("wget", "/run/net/http", "HTTP download service is not available");
}
