#include "../common.h"

#include <errno.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>

static int list_dir(const char *path, int show_header) {
    DIR *dir = opendir(path);
    struct dirent *entry;

    if (dir == NULL) {
        fprintf(stderr, "ls: %s: %s\n", path, strerror(errno));
        return 1;
    }

    if (show_header)
        printf("%s:\n", path);

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_name[0] == '.')
            continue;
        puts(entry->d_name);
    }

    closedir(dir);
    return 0;
}

int main(int argc, char *argv[]) {
    int status = 0;

    if (util_is_help(argc, argv)) {
        util_usage("ls", "[directory]...");
        return 0;
    }

    if (argc == 1)
        return list_dir(".", 0);

    for (int i = 1; i < argc; ++i)
        status |= list_dir(argv[i], argc > 2);

    return status ? 1 : 0;
}
