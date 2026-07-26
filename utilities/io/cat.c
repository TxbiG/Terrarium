#include "../common.h"

#include <errno.h>
#include <stdio.h>
#include <string.h>

static int cat_file(const char *path) {
    FILE *fp = fopen(path, "rb");
    int c;

    if (!fp) {
        fprintf(stderr, "cat: %s: %s\n", path, strerror(errno));
        return 1;
    }

    while ((c = fgetc(fp)) != EOF)
        putchar(c);

    if (ferror(fp)) {
        fprintf(stderr, "cat: %s: read error\n", path);
        fclose(fp);
        return 1;
    }

    fclose(fp);
    return 0;
}

int main(int argc, char *argv[]) {
    int status = 0;

    if (util_is_help(argc, argv)) {
        util_usage("cat", "<file>...");
        return 0;
    }

    if (argc < 2) {
        util_usage("cat", "<file>...");
        return 1;
    }

    for (int i = 1; i < argc; ++i)
        status |= cat_file(argv[i]);

    return status ? 1 : 0;
}
