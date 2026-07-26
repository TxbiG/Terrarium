#include "../common.h"

int main(int argc, char *argv[]) {
    if (util_is_help(argc, argv)) {
        util_usage("ktrace", "[events]");
        return 0;
    }

    return util_print_file("ktrace", argc > 1 ? argv[1] : "/proc/trace");
}
