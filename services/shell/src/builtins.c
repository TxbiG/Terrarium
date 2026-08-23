#ifdef _WIN32
#include <direct.h>
#define chdir _chdir
#define getcwd _getcwd
#else
#include <unistd.h>
#endif
#include <stdlib.h>
#include <stdio.h>

#include <shell/builtins.h>
#include <shell/shell.h>

int shell_builtin_cd(int argc, char **argv) {
    const char *target = argv[1];

    if (!target)
        target = getenv("HOME");

    if (!target) {
        fprintf(stderr, "cd: missing directory\n");
        return -1;
    }

    if (chdir(target) != 0) {
        perror("cd");
        return 1;
    }

    return 0;
}

int shell_builtin_exit(int argc, char **argv) {
    (void)argc;
    (void)argv;
    exit(0);
}

int shell_builtin_pwd(int argc, char **argv) {
    char cwd[1024];
    (void)argc;
    (void)argv;

    if (!getcwd(cwd, sizeof(cwd))) {
        perror("pwd");
        return 1;
    }

    puts(cwd);
    return 0;
}

int shell_builtin_help(int argc, char **argv) {
    (void)argc;
    (void)argv;

    puts("Terrarium shell builtins: cd, exit, help, pwd, source");
    return 0;
}

int shell_builtin_source(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "source: missing file\n");
        return 1;
    }

    if (shell_run_script(argv[1]) != 0) {
        perror(argv[1]);
        return 1;
    }

    return 0;
}
