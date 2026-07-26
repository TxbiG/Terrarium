#include "main.h"
#include "common/service_runtime.h"

static int _ready;

int fs_service_start(void) {
    _ready = 1;
    return 0;
}

int fs_service_status(void) {
    return _ready;
}

int main(int argc, char *argv[]) {
    return terrarium_service_main("fs", argc, argv, fs_service_start, fs_service_status);
}