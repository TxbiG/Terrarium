#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <sys/wait.h>

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

int main(int argc, char **argv) {
    pid_t pid;

    // Launch essential services here (omitted for brevity)
    // e.g. network, logging daemons

    // Launch Wayland compositor (Hyprland)
    pid = fork();
    if (pid == 0) {
        // Child: set env vars needed by compositor
        setenv("XDG_SESSION_TYPE", "wayland", 1);
        setenv("XDG_CURRENT_DESKTOP", "Hyprland", 1);
        setenv("QT_QPA_PLATFORM", "wayland", 1);

        execl("/usr/bin/Hyprland", "Hyprland", NULL);
        perror("Failed to exec Hyprland");
        exit(1);
    }

    // Parent waits for compositor to exit
    int status;
    waitpid(pid, &status, 0);

    // If compositor exits, fallback to shell
    execl("/bin/sh", "sh", NULL);
    perror("Failed to exec shell");
    return 1;
}

#ifdef __cplusplus
}
#endif  // __cplusplus