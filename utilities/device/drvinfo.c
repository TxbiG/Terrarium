#include "../common.h"

int main(int argc, char **argv) {
    if (util_is_help(argc, argv)) {
        util_usage("drvinfo", "[driver]");
        return 0;
    }
    return util_print_file("drvinfo", "/sys/drivers");
}
