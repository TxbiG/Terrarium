#include "../common.h"

#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

static int find_path(const char *path, const char *name_filter) {
    struct stat st;
    if (stat(path, &st) != 0) {
        fprintf(stderr, "find: %s: %s\n", path, strerror(errno));
        return 1;
    }

    const char *base = strrchr(path, '/');
    base = base ? base + 1 : path;
    if (!name_filter || strstr(base, name_filter))
        puts(path);

    if (!S_ISDIR(st.st_mode))
        return 0;

    DIR *dir = opendir(path);
    if (!dir)
        return 1;

    struct dirent *entry;
    int status = 0;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        char child[4096];
        snprintf(child, sizeof(child), "%s/%s", path, entry->d_name);
        status |= find_path(child, name_filter);
    }

    closedir(dir);
    return status;
}

int main(int argc, char *argv[]) {
    const char *root = ".";
    const char *name = 0;

    if (util_is_help(argc, argv)) {
        util_usage("find", "[path] [-name text]");
        return 0;
    }

    if (argc > 1)
        root = argv[1];
    if (argc > 3 && strcmp(argv[2], "-name") == 0)
        name = argv[3];

    return find_path(root, name);
}
