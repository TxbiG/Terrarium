#include "../common.h"

#include <libgen.h>

int main(int argc, char **argv) {
    if (util_is_help(argc, argv)) { util_usage("basename", "PATH"); return 0; }
    if (argc != 2) { util_usage("basename", "PATH"); return 1; }
    char *copy = strdup(argv[1]);
    if (!copy) return 1;
    puts(basename(copy));
    free(copy);
    return 0;
}
