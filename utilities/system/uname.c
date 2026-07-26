#include "../common.h"

#include <stdio.h>
#include <sys/utsname.h>

int main(int argc, char *argv[]) {
    struct utsname info;

    if (util_is_help(argc, argv)) {
        util_usage("uname", "[-a]");
        return 0;
    }

    if (uname(&info) != 0) {
        perror("uname");
        return 1;
    }

    if (argc > 1 && argv[1][0] == '-' && argv[1][1] == 'a') {
        printf("%s %s %s %s %s\n",
               info.sysname, info.nodename, info.release,
               info.version, info.machine);
    } else {
        puts(info.sysname);
    }

    return 0;
}
