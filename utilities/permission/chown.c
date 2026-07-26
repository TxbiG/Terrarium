#include "../common.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (util_is_help(argc, argv)) {
        util_usage("chown", "<uid>[:gid] <file>...");
        return 0;
    }

    if (argc < 3) {
        util_usage("chown", "<uid>[:gid] <file>...");
        return 1;
    }

    char *group = strchr(argv[1], ':');
    if (group)
        *group++ = '\0';

    uid_t uid = (uid_t)strtoul(argv[1], 0, 10);
    gid_t gid = group && *group ? (gid_t)strtoul(group, 0, 10) : (gid_t)-1;

    int status = 0;
    for (int i = 2; i < argc; ++i) {
        if (chown(argv[i], uid, gid) != 0) {
            fprintf(stderr, "chown: %s: %s\n", argv[i], strerror(errno));
            status = 1;
        }
    }
    return status;
}
