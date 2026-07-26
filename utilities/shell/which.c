#include "../common.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static int find_command(const char *name) {
    const char *path = getenv("PATH");
    char candidate[1024];
    const char *start;
    const char *end;

    if (strchr(name, '/')) {
        if (access(name, X_OK) == 0) {
            puts(name);
            return 0;
        }
        return 1;
    }

    if (!path)
        path = "/bin:/usr/bin";

    start = path;
    while (*start) {
        end = strchr(start, ':');
        if (!end)
            end = start + strlen(start);

        if ((size_t)(end - start) + strlen(name) + 2 < sizeof(candidate)) {
            snprintf(candidate, sizeof(candidate), "%.*s/%s",
                     (int)(end - start), start, name);
            if (access(candidate, X_OK) == 0) {
                puts(candidate);
                return 0;
            }
        }

        start = *end ? end + 1 : end;
    }

    return 1;
}

int main(int argc, char *argv[]) {
    int status = 0;

    if (util_is_help(argc, argv)) {
        util_usage("which", "<command>...");
        return 0;
    }

    if (argc < 2) {
        util_usage("which", "<command>...");
        return 1;
    }

    for (int i = 1; i < argc; ++i)
        status |= find_command(argv[i]);

    return status ? 1 : 0;
}
