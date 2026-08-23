#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <shell/shell.h>
#include <shell/exec.h>

static void prompt(void) {
    printf("terrarium$ ");
    fflush(stdout);
}

int shell_run_line(char *line) {
    char *argv[32];
    int argc = 0;
    char *tok;

    if (!line)
        return 0;

    tok = strtok(line, " \t\r\n");
    while (tok && argc < 31) {
        argv[argc++] = tok;
        tok = strtok(NULL, " \t\r\n");
    }
    argv[argc] = NULL;

    if (argc == 0)
        return 0;

    return shell_exec(argv);
}

void shell_run(void) {
    char line[1024];

    while (1) {
        prompt();

        if (!fgets(line, sizeof(line), stdin))
            break;

        if (line[0] == '\n' || line[0] == '#')
            continue;

        shell_run_line(line);
    }
}
