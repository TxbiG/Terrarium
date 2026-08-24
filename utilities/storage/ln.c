#include "../common.h"

#include <unistd.h>

int main(int argc, char **argv) {
    if (util_is_help(argc, argv)) { util_usage("ln", "[-s] TARGET LINK_NAME"); return 0; }
    int symbolic = 0, first = 1;
    if (argc > 1 && strcmp(argv[1], "-s") == 0) { symbolic = 1; first = 2; }
    if (argc - first != 2) { util_usage("ln", "[-s] TARGET LINK_NAME"); return 1; }

    int rc = symbolic ? symlink(argv[first], argv[first + 1])
                      : link(argv[first], argv[first + 1]);
    if (rc != 0) {
        fprintf(stderr, "ln: %s\n", strerror(errno));
        return 1;
    }
    return 0;
}
