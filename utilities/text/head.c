#include "../common.h"

#include <errno.h>
#include <stdio.h>
#include <string.h>

static int head_stream(FILE *file, const char *label, int show_label) {
    char line[4096];

    if (show_label)
        printf("==> %s <==\n", label);

    for (int i = 0; i < 10 && fgets(line, sizeof(line), file); ++i)
        fputs(line, stdout);

    return 0;
}

int main(int argc, char *argv[]) {
    int status = 0;

    if (util_is_help(argc, argv)) {
        util_usage("head", "[file]...");
        return 0;
    }

    if (argc == 1)
        return head_stream(stdin, "", 0);

    for (int i = 1; i < argc; ++i) {
        FILE *file = fopen(argv[i], "r");
        if (!file) {
            fprintf(stderr, "head: %s: %s\n", argv[i], strerror(errno));
            status = 1;
            continue;
        }
        head_stream(file, argv[i], argc > 2);
        fclose(file);
    }

    return status;
}
