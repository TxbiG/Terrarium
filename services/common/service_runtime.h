#ifndef TERRARIUM_SERVICE_RUNTIME_H
#define TERRARIUM_SERVICE_RUNTIME_H

#include <stdio.h>
#include <string.h>

typedef int (*terrarium_service_start_fn)(void);
typedef int (*terrarium_service_status_fn)(void);

static inline int terrarium_service_main(
    const char *name,
    int argc,
    char **argv,
    terrarium_service_start_fn start,
    terrarium_service_status_fn status) {
    if (argc > 1) {
        if (strcmp(argv[1], "--help") == 0) {
            printf("usage: %s [--status] [--run-once] [--help]\n", argv[0]);
            return 0;
        }
        if (strcmp(argv[1], "--status") == 0) {
            int ready = status ? status() : 0;
            printf("%s: %s\n", name, ready ? "running" : "stopped");
            return ready ? 0 : 1;
        }
        if (strcmp(argv[1], "--run-once") != 0) {
            printf("%s: unknown option %s\n", name, argv[1]);
            return 2;
        }
    }

    if (start && start() != 0) {
        printf("%s: failed\n", name);
        return 1;
    }

    printf("%s: ready\n", name);
    return 0;
}

#endif