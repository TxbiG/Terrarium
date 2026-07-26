#include "../common.h"

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

static int print_usage_for_path(const char *path) {
    struct stat st;

    if (stat(path, &st) != 0) {
        fprintf(stderr, "du: %s: %s\n", path, strerror(errno));
        return 1;
    }

    printf("%llu\t%s\n", (unsigned long long)st.st_size, path);
    return 0;
}

int main(int argc, char *argv[]) {
    int status = 0;

    if (util_is_help(argc, argv)) {
        util_usage("du", "[path]...");
        return 0;
    }

    if (argc == 1)
        return print_usage_for_path(".");

    for (int i = 1; i < argc; ++i)
        status |= print_usage_for_path(argv[i]);

    return status ? 1 : 0;
}
