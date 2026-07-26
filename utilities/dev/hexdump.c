#include "../common.h"

#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

static int dump_file(const char *path) {
    FILE *file = fopen(path, "rb");
    unsigned char buffer[16];
    unsigned long offset = 0;

    if (!file) {
        fprintf(stderr, "hexdump: %s: %s\n", path, strerror(errno));
        return 1;
    }

    while (!feof(file)) {
        size_t n = fread(buffer, 1, sizeof(buffer), file);
        if (n == 0)
            break;

        printf("%08lx  ", offset);
        for (size_t i = 0; i < sizeof(buffer); ++i) {
            if (i < n)
                printf("%02x ", buffer[i]);
            else
                fputs("   ", stdout);
            if (i == 7)
                fputc(' ', stdout);
        }

        fputs(" |", stdout);
        for (size_t i = 0; i < n; ++i)
            fputc(isprint(buffer[i]) ? buffer[i] : '.', stdout);
        puts("|");
        offset += (unsigned long)n;
    }

    fclose(file);
    return 0;
}

int main(int argc, char *argv[]) {
    int status = 0;

    if (util_is_help(argc, argv)) {
        util_usage("hexdump", "<file>...");
        return 0;
    }

    if (argc < 2) {
        util_usage("hexdump", "<file>...");
        return 1;
    }

    for (int i = 1; i < argc; ++i)
        status |= dump_file(argv[i]);

    return status ? 1 : 0;
}
