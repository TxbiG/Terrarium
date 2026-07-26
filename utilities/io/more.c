#include "../common.h"

#include <errno.h>
#include <stdio.h>
#include <string.h>

static int print_file(const char *path) {
    FILE *file = fopen(path, "r");
    char buffer[4096];

    if (!file) {
        fprintf(stderr, "more: %s: %s\n", path, strerror(errno));
        return 1;
    }

    while (fgets(buffer, sizeof(buffer), file))
        fputs(buffer, stdout);

    fclose(file);
    return 0;
}

int main(int argc, char *argv[]) {
    if (util_is_help(argc, argv)) {
        util_usage("more", "[file]...");
        return 0;
    }

    if (argc == 1) {
        char buffer[4096];
        while (fgets(buffer, sizeof(buffer), stdin))
            fputs(buffer, stdout);
        return 0;
    }

    int status = 0;
    for (int i = 1; i < argc; ++i)
        status |= print_file(argv[i]);

    return status ? 1 : 0;
}
