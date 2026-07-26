#include "../common.h"

#include <errno.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (util_is_help(argc, argv)) {
        util_usage("diff", "<left> <right>");
        return 0;
    }

    if (argc != 3) {
        util_usage("diff", "<left> <right>");
        return 1;
    }

    FILE *left = fopen(argv[1], "r");
    if (!left) {
        fprintf(stderr, "diff: %s: %s\n", argv[1], strerror(errno));
        return 1;
    }

    FILE *right = fopen(argv[2], "r");
    if (!right) {
        fprintf(stderr, "diff: %s: %s\n", argv[2], strerror(errno));
        fclose(left);
        return 1;
    }

    char a[4096];
    char b[4096];
    unsigned long line = 1;
    int different = 0;

    for (;;) {
        char *la = fgets(a, sizeof(a), left);
        char *lb = fgets(b, sizeof(b), right);
        if (!la && !lb)
            break;
        if (!la || !lb || strcmp(a, b) != 0) {
            printf("%lu\n", line);
            if (la)
                printf("< %s", a);
            if (lb)
                printf("> %s", b);
            different = 1;
        }
        ++line;
    }

    fclose(left);
    fclose(right);
    return different ? 1 : 0;
}
