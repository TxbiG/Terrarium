#include "../common.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAIL_MAX_LINES 1024
#define TAIL_LINE_SIZE 4096

static int tail_stream(FILE *file, const char *label, int show_label, int lines) {
    static char ring[TAIL_MAX_LINES][TAIL_LINE_SIZE];
    int count = 0;
    int next = 0;

    while (fgets(ring[next], TAIL_LINE_SIZE, file)) {
        next = (next + 1) % lines;
        if (count < lines)
            ++count;
    }

    if (show_label)
        printf("==> %s <==\n", label);

    int start = (next - count + lines) % lines;
    for (int i = 0; i < count; ++i)
        fputs(ring[(start + i) % lines], stdout);

    return 0;
}

int main(int argc, char *argv[]) {
    int lines = 10;
    int first_file = 1;

    if (util_is_help(argc, argv)) {
        util_usage("tail", "[-n lines] [file]...");
        return 0;
    }

    if (argc > 2 && strcmp(argv[1], "-n") == 0) {
        lines = atoi(argv[2]);
        if (lines <= 0 || lines > TAIL_MAX_LINES)
            lines = 10;
        first_file = 3;
    }

    if (first_file >= argc)
        return tail_stream(stdin, "", 0, lines);

    int status = 0;
    for (int i = first_file; i < argc; ++i) {
        FILE *file = fopen(argv[i], "r");
        if (!file) {
            fprintf(stderr, "tail: %s: %s\n", argv[i], strerror(errno));
            status = 1;
            continue;
        }
        tail_stream(file, argv[i], argc - first_file > 1, lines);
        fclose(file);
    }

    return status;
}
