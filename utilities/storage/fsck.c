#include "../common.h"

int main(int argc, char *argv[]) {
    if (util_is_help(argc, argv) || argc < 2) {
        util_usage("fsck", "<device>");
        return argc < 2 ? 1 : 0;
    }

    return util_backend_missing("fsck", argv[1], "filesystem check backends are not installed yet");
}
