#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <shell/exec.h>
#include <shell/builtins.h>
#include <string.h>

int shell_exec(char **argv) {
    if (!argv[0])
        return 0;

    int argc = 0;
    while (argv[argc])
        argc++;

    if (strcmp(argv[0], "cd") == 0)
        return shell_builtin_cd(argc, argv);

    if (strcmp(argv[0], "exit") == 0)
        return shell_builtin_exit(argc, argv);

    if (strcmp(argv[0], "pwd") == 0)
        return shell_builtin_pwd(argc, argv);

    if (strcmp(argv[0], "help") == 0)
        return shell_builtin_help(argc, argv);

    if (strcmp(argv[0], "source") == 0 || strcmp(argv[0], ".") == 0)
        return shell_builtin_source(argc, argv);

    pid_t pid = fork();
    if (pid == 0) {
        execvp(argv[0], argv);
        perror(argv[0]);
        _exit(127);
    } else if (pid > 0) {
        int status = 0;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status))
            return WEXITSTATUS(status);
        if (WIFSIGNALED(status))
            return 128 + WTERMSIG(status);
    } else {
        perror("fork");
        return 1;
    }

    return 0;
}
