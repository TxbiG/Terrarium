#include "session.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int login_session_start(const login_account_t *account) {
    const char *shell;

    if (!account)
        return -1;

    shell = account->shell[0] ? account->shell : "/usr/bin/terrarium-shell";

    setenv("USER", account->name, 1);
    setenv("HOME", account->home, 1);
    setenv("SHELL", shell, 1);

    /* Profile execution will eventually move into the shell/session runtime. */
    if (system("/services/shell/scripts/profile.sh") != 0)
        printf("warning: profile script failed or is unavailable\n");

    if (fork() == 0) {
        execl("/usr/sbin/terrarium-lumina", "terrarium-lumina", NULL);
        _exit(1);
    }

    execl(shell, "terrarium-shell", NULL);
    return -1;
}