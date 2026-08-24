#include "../common.h"

#include <fcntl.h>
#include <sys/stat.h>
#include <utime.h>

int main(int argc, char **argv) {
    if (util_is_help(argc, argv)) { util_usage("touch", "FILE..."); return 0; }
    if (argc < 2) { util_usage("touch", "FILE..."); return 1; }

    int status = 0;
    for (int i = 1; i < argc; ++i) {
        int fd = open(argv[i], O_WRONLY | O_CREAT, 0666);
        if (fd < 0) {
            fprintf(stderr, "touch: %s: %s\n", argv[i], strerror(errno));
            status = 1;
            continue;
        }
        close(fd);
        if (utime(argv[i], NULL) != 0) {
            fprintf(stderr, "touch: %s: %s\n", argv[i], strerror(errno));
            status = 1;
        }
    }
    return status;
}
