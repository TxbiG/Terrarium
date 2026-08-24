#include "../common.h"

#include <stdlib.h>

int main(int argc, char **argv) {
    if (util_is_help(argc, argv)) { util_usage("tee", "[-a] [FILE]..."); return 0; }

    int append = 0, first = 1;
    if (argc > 1 && strcmp(argv[1], "-a") == 0) { append = 1; first = 2; }

    FILE **out = NULL;
    size_t count = (size_t)(argc - first);
    if (count) {
        out = calloc(count, sizeof(*out));
        if (!out) return 1;
        for (size_t i = 0; i < count; ++i) {
            out[i] = fopen(argv[first + (int)i], append ? "a" : "w");
            if (!out[i]) {
                fprintf(stderr, "tee: %s: %s\n", argv[first + (int)i], strerror(errno));
                count = i;
                break;
            }
        }
    }

    int c;
    while ((c = getchar()) != EOF) {
        putchar(c);
        for (size_t i = 0; i < count; ++i) fputc(c, out[i]);
    }
    for (size_t i = 0; i < count; ++i) fclose(out[i]);
    free(out);
    return 0;
}
