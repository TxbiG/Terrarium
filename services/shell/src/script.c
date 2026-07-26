#include <stdio.h>
#include <string.h>
#include <shell/shell.h>

int shell_run_script(const char *path) {
    FILE *f = fopen(path, "r");
    if (!f) return -1;

    char line[1024];
    while (fgets(line, sizeof(line), f)) {
        if (line[0] == '#' || line[0] == '\n')
            continue;
        shell_run_line(line);
    }
    fclose(f);
    return 0;
}
