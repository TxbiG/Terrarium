#include "../common.h"

#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

static int wc_stream(FILE *file, const char *label, int show_label) {
    unsigned long lines = 0;
    unsigned long words = 0;
    unsigned long bytes = 0;
    int in_word = 0;
    int ch;

    while ((ch = fgetc(file)) != EOF) {
        ++bytes;
        if (ch == '\n')
            ++lines;
        if (isspace(ch)) {
            in_word = 0;
        } else if (!in_word) {
            in_word = 1;
            ++words;
        }
    }

    printf("%lu %lu %lu", lines, words, bytes);
    if (show_label)
        printf(" %s", label);
    putchar('\n');
    return 0;
}

int main(int argc, char *argv[]) {
    int status = 0;

    if (util_is_help(argc, argv)) {
        util_usage("wc", "[file]...");
        return 0;
    }

    if (argc == 1)
        return wc_stream(stdin, "", 0);

    for (int i = 1; i < argc; ++i) {
        FILE *file = fopen(argv[i], "rb");
        if (!file) {
            fprintf(stderr, "wc: %s: %s\n", argv[i], strerror(errno));
            status = 1;
            continue;
        }
        wc_stream(file, argv[i], 1);
        fclose(file);
    }

    return status;
}
