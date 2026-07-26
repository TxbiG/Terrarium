#include "../common.h"

extern char **environ;

int main(int argc, char *argv[]) {
    char **entry;

    if (util_is_help(argc, argv)) {
        util_usage("set", "");
        return 0;
    }

    for (entry = environ; entry && *entry; ++entry)
        puts(*entry);
    return 0;
}
