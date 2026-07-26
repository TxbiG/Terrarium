#include "../common.h"

int main(int argc, char *argv[]) {
    if (util_is_help(argc, argv)) {
        util_usage("export", "[name=value]...");
        return 0;
    }

    return util_backend_missing("export", "process environment", "export must be handled as a shell builtin");
}
