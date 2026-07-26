#include "../common.h"

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

static int stat_path(const char *path) {
    struct stat st;

    if (stat(path, &st) != 0) {
        fprintf(stderr, "stat: %s: %s\n", path, strerror(errno));
        return 1;
    }

    printf("File: %s\n", path);
    printf("Size: %llu\n", (unsigned long long)st.st_size);
    printf("Mode: %o\n", (unsigned int)st.st_mode);
    return 0;
}

int main(int argc, char *argv[]) {
    int status = 0;

    if (util_is_help(argc, argv)) {
        util_usage("stat", "<path>...");
        return 0;
    }

    if (argc < 2) {
        util_usage("stat", "<path>...");
        return 1;
    }

    for (int i = 1; i < argc; ++i)
        status |= stat_path(argv[i]);

    return status ? 1 : 0;
}
