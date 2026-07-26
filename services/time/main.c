#include "main.h"
#include "common/service_runtime.h"

#include <stdio.h>
#include <time.h>

static int time_ready;

int time_service_start(void) {
    time_ready = 1;
    return 0;
}

int time_service_status(void) {
    return time_ready;
}

int main(int argc, char *argv[]) {
    int result = terrarium_service_main("time", argc, argv, time_service_start, time_service_status);
    if (result == 0 && argc <= 1)
        printf("time: now %ld\n", (long)time(0));
    return result;
}