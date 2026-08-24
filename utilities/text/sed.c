#include "../common.h"

#include <stdlib.h>

int main(int argc, char **argv) {
    if (util_is_help(argc, argv)) { util_usage("sed", "s/OLD/NEW/[g] [FILE]..."); return 0; }
    if (argc < 2) { util_usage("sed", "s/OLD/NEW/[g] [FILE]..."); return 1; }

    const char *expr = argv[1];
    if (expr[0] != 's' || expr[1] != '/' || strchr(expr + 2, '/') == NULL) {
        fprintf(stderr, "sed: only basic s/OLD/NEW/[g] is supported initially\n");
        return 1;
    }

    char *spec = strdup(expr + 2);
    if (!spec) return 1;
    char *old = spec;
    char *slash = strchr(old, '/');
    *slash++ = '\0';
    char *newtext = slash;
    char *last = strrchr(newtext, '/');
    if (!last) { free(spec); return 1; }
    *last = '\0';
    int global = strcmp(last + 1, "g") == 0;

    FILE *in = stdin;
    int status = 0;
    if (argc > 2) {
        in = fopen(argv[2], "r");
        if (!in) { fprintf(stderr, "sed: %s: %s\n", argv[2], strerror(errno)); free(spec); return 1; }
    }

    char line[8192];
    while (fgets(line, sizeof(line), in)) {
        char *p = line;
        while (*p) {
            char *hit = strstr(p, old);
            if (!hit) { fputs(p, stdout); break; }
            fwrite(p, 1, (size_t)(hit - p), stdout);
            fputs(newtext, stdout);
            p = hit + strlen(old);
            if (!global) { fputs(p, stdout); break; }
        }
    }
    if (in != stdin) fclose(in);
    free(spec);
    return status;
}
