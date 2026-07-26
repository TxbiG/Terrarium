#include "../common.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SORT_MAX_LINES 8192
#define SORT_LINE_SIZE 4096

static int line_compare(const void *left, const void *right) {
    const char *const *a = (const char *const *)left;
    const char *const *b = (const char *const *)right;
    return strcmp(*a, *b);
}

static int sort_stream(FILE *file) {
    char *lines[SORT_MAX_LINES];
    char buffer[SORT_LINE_SIZE];
    size_t count = 0;

    while (count < SORT_MAX_LINES && fgets(buffer, sizeof(buffer), file)) {
        size_t len = strlen(buffer) + 1;
        lines[count] = (char *)malloc(len);
        if (!lines[count])
            return 1;
        memcpy(lines[count], buffer, len);
        ++count;
    }

    qsort(lines, count, sizeof(lines[0]), line_compare);
    for (size_t i = 0; i < count; ++i) {
        fputs(lines[i], stdout);
        free(lines[i]);
    }

    return 0;
}

int main(int argc, char *argv[]) {
    if (util_is_help(argc, argv)) {
        util_usage("sort", "[file]...");
        return 0;
    }

    if (argc == 1)
        return sort_stream(stdin);

    int status = 0;
    for (int i = 1; i < argc; ++i) {
        FILE *file = fopen(argv[i], "r");
        if (!file) {
            fprintf(stderr, "sort: %s: %s\n", argv[i], strerror(errno));
            status = 1;
            continue;
        }
        status |= sort_stream(file);
        fclose(file);
    }

    return status ? 1 : 0;
}
