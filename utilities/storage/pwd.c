#include "../common.h"

#include <limits.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    char path[PATH_MAX];

    if (util_is_help(argc, argv)) {
        util_usage("pwd", "");
        return 0;
    }

    if (!getcwd(path, sizeof(path))) {
        perror("pwd");
        return 1;
    }

    puts(path);
    return 0;
}
