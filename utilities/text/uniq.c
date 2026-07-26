#include "../common.h"

#include <errno.h>
#include <stdio.h>
#include <string.h>

static int uniq_stream(FILE *file) {
    char previous[4096] = {0};
    char current[4096];
    int have_previous = 0;

    while (fgets(current, sizeof(current), file)) {
        if (!have_previous || strcmp(previous, current) != 0) {
            fputs(current, stdout);
            strcpy(previous, current);
            have_previous = 1;
        }
    }

    return 0;
}

int main(int argc, char *argv[]) {
    if (util_is_help(argc, argv)) {
        util_usage("uniq", "[file]");
        return 0;
    }

    if (argc == 1)
        return uniq_stream(stdin);

    FILE *file = fopen(argv[1], "r");
    if (!file) {
        fprintf(stderr, "uniq: %s: %s\n", argv[1], strerror(errno));
        return 1;
    }

    int status = uniq_stream(file);
    fclose(file);
    return status;
}
