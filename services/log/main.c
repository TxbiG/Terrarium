#include "main.h"
#include "common/service_runtime.h"

#include <stdio.h>

static int log_ready;

int log_service_start(void) {
    log_ready = 1;
    return 0;
}

int log_service_status(void) {
    return log_ready;
}

int log_service_write(const char *message) {
    if (!log_ready || !message)
        return -1;
    puts(message);
    return 0;
}

int main(int argc, char *argv[]) {
    return terrarium_service_main("log", argc, argv, log_service_start, log_service_status);
}