#include "../common.h"

#include <sys/stat.h>

static const char *kind(mode_t mode) {
    if (S_ISREG(mode)) return "regular file";
    if (S_ISDIR(mode)) return "directory";
    if (S_ISLNK(mode)) return "symbolic link";
    if (S_ISCHR(mode)) return "character device";
    if (S_ISBLK(mode)) return "block device";
    if (S_ISFIFO(mode)) return "FIFO";
    if (S_ISSOCK(mode)) return "socket";
    return "unknown";
}

int main(int argc, char **argv) {
    struct stat st;
    if (util_is_help(argc, argv)) { util_usage("file", "FILE..."); return 0; }
    if (argc < 2) { util_usage("file", "FILE..."); return 1; }

    int status = 0;
    for (int i = 1; i < argc; ++i) {
        if (lstat(argv[i], &st) != 0) {
            fprintf(stderr, "file: %s: %s\n", argv[i], strerror(errno));
            status = 1;
            continue;
        }
        printf("%s: %s\n", argv[i], kind(st.st_mode));
    }
    return status;
}
