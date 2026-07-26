#include "../common.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void cut_stream(FILE *file, int start, int end) {
    char line[4096];
    while (fgets(line, sizeof(line), file)) {
        int len = (int)strlen(line);
        int stop = end > len ? len : end;
        for (int i = start; i < stop; ++i)
            fputc(line[i], stdout);
        if (len > 0 && line[len - 1] == '\n')
            fputc('\n', stdout);
    }
}

int main(int argc, char *argv[]) {
    if (util_is_help(argc, argv)) {
        util_usage("cut", "-c start-end [file]");
        return 0;
    }

    if (argc < 3 || strcmp(argv[1], "-c") != 0) {
        util_usage("cut", "-c start-end [file]");
        return 1;
    }

    int start = atoi(argv[2]);
    char *dash = strchr(argv[2], '-');
    int end = dash ? atoi(dash + 1) : start;
    if (start <= 0 || end < start) {
        util_usage("cut", "-c start-end [file]");
        return 1;
    }
    --start;

    if (argc == 3) {
        cut_stream(stdin, start, end);
        return 0;
    }

    FILE *file = fopen(argv[3], "r");
    if (!file) {
        fprintf(stderr, "cut: %s: %s\n", argv[3], strerror(errno));
        return 1;
    }
    cut_stream(file, start, end);
    fclose(file);
    return 0;
}
