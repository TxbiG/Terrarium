#include <shell/shell.h>
#include <stdio.h>

int main(int argc, char **argv) {
    if (argc > 1) {
        if (shell_run_script(argv[1]) != 0) {
            fprintf(stderr, "terrarium-shell: failed to run %s\n", argv[1]);
            return 1;
        }
        return 0;
    }

    shell_run();
    return 0;
}
