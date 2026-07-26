#ifndef TERRARIUM_UTIL_COMMON_H
#define TERRARIUM_UTIL_COMMON_H

#include <stdio.h>
#include <string.h>
#include <errno.h>

static int util_is_help(int argc, char **argv) { return argc > 1 && (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0); }

static void util_usage(const char *name, const char *usage) { fprintf(stderr, "Usage: %s %s\n", name, usage); }

static int util_backend_missing(const char *name, const char *path, const char *detail) {
    fprintf(stderr, "%s: Terrarium backend unavailable", name);
    if (path && path[0])
        fprintf(stderr, " (%s)", path);
    if (detail && detail[0])
        fprintf(stderr, ": %s", detail);
    fputc('\n', stderr);
    return 2;
}

static int util_print_file(const char *name, const char *path) {
    FILE *file = fopen(path, "rb");
    char buffer[4096];
    size_t got;

    if (!file)
        return util_backend_missing(name, path, strerror(errno));

    while ((got = fread(buffer, 1, sizeof(buffer), file)) > 0) {
        if (fwrite(buffer, 1, got, stdout) != got) {
            fclose(file);
            fprintf(stderr, "%s: write failed\n", name);
            return 1;
        }
    }

    if (ferror(file)) {
        fprintf(stderr, "%s: %s: %s\n", name, path, strerror(errno));
        fclose(file);
        return 1;
    }

    fclose(file);
    return 0;
}

#endif
