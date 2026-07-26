#include "../common.h"

#include <errno.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
    FILE *src;
    FILE *dst;
    char buffer[4096];
    size_t n;

    if (util_is_help(argc, argv)) {
        util_usage("cp", "<source> <destination>");
        return 0;
    }

    if (argc != 3) {
        util_usage("cp", "<source> <destination>");
        return 1;
    }

    src = fopen(argv[1], "rb");
    if (!src) {
        fprintf(stderr, "cp: %s: %s\n", argv[1], strerror(errno));
        return 1;
    }

    dst = fopen(argv[2], "wb");
    if (!dst) {
        fprintf(stderr, "cp: %s: %s\n", argv[2], strerror(errno));
        fclose(src);
        return 1;
    }

    while ((n = fread(buffer, 1, sizeof(buffer), src)) > 0) {
        if (fwrite(buffer, 1, n, dst) != n) {
            fprintf(stderr, "cp: %s: write error\n", argv[2]);
            fclose(src);
            fclose(dst);
            return 1;
        }
    }

    if (ferror(src)) {
        fprintf(stderr, "cp: %s: read error\n", argv[1]);
        fclose(src);
        fclose(dst);
        return 1;
    }

    fclose(src);
    fclose(dst);
    return 0;
}
