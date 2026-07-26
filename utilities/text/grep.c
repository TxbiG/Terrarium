#include "../common.h"

#include <errno.h>
#include <stdio.h>
#include <string.h>

static int grep_stream(const char *pattern, FILE *file, const char *label, int show_label) {
    char line[4096];
    int matched = 0;

    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, pattern)) {
            if (show_label)
                printf("%s:", label);
            fputs(line, stdout);
            matched = 1;
        }
    }

    return matched ? 0 : 1;
}

int main(int argc, char *argv[]) {
    int status = 1;

    if (util_is_help(argc, argv)) {
        util_usage("grep", "<pattern> [file]...");
        return 0;
    }

    if (argc < 2) {
        util_usage("grep", "<pattern> [file]...");
        return 1;
    }

    if (argc == 2)
        return grep_stream(argv[1], stdin, "", 0);

    for (int i = 2; i < argc; ++i) {
        FILE *file = fopen(argv[i], "r");
        if (!file) {
            fprintf(stderr, "grep: %s: %s\n", argv[i], strerror(errno));
            continue;
        }
        if (grep_stream(argv[1], file, argv[i], argc > 3) == 0)
            status = 0;
        fclose(file);
    }

    return status;
}
