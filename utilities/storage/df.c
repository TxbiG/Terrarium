#include "../common.h"

#include <stdio.h>

int main(int argc, char *argv[]) {
    if (util_is_help(argc, argv)) {
        util_usage("df", "[path]");
        return 0;
    }

    const char *path = argc > 1 ? argv[1] : "/";
    printf("Filesystem  Used  Available  Mounted on\n");
    printf("terrarium   n/a   n/a        %s\n", path);
    return 0;
}
