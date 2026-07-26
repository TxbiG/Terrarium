#include "main.h"
#include "common/service_runtime.h"

static int _ready;

int launcher_service_start(void) {
    _ready = 1;
    return 0;
}

int launcher_service_status(void) {
    return _ready;
}

int main(int argc, char *argv[]) {
    return terrarium_service_main("launcher", argc, argv, launcher_service_start, launcher_service_status);
}