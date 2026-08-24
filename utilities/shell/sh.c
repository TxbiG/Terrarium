#include "../common.h"

#include <stdlib.h>

static void run_line(const char *line) {
    /* Placeholder command dispatcher. Replace with Terrarium's shell parser/exec API. */
    printf("sh: command dispatcher not connected: %s", line);
}

int main(int argc, char **argv) {
    if (util_is_help(argc, argv)) { util_usage("sh", "[SCRIPT]"); return 0; }

    if (argc > 1) {
        FILE *f = fopen(argv[1], "r");
        if (!f) {
            fprintf(stderr, "sh: %s: %s\n", argv[1], strerror(errno));
            return 1;
        }
        char line[4096];
        while (fgets(line, sizeof(line), f)) run_line(line);
        fclose(f);
        return 0;
    }

    char line[4096];
    while (fputs("$ ", stdout), fflush(stdout), fgets(line, sizeof(line), stdin))
        run_line(line);
    return 0;
}
