#include "main.h"
#include "common/service_runtime.h"

static int _ready;

int input_service_start(void) {
    _ready = 1;
    return 0;
}

int input_service_status(void) {
    return _ready;
}

int main(int argc, char *argv[]) {
    return terrarium_service_main("input", argc, argv, input_service_start, input_service_status);
}