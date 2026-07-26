#include "main.h"
#include "common/service_runtime.h"

static int _ready;

int audio_service_start(void) {
    _ready = 1;
    return 0;
}

int audio_service_status(void) {
    return _ready;
}

int main(int argc, char *argv[]) {
    return terrarium_service_main("audio", argc, argv, audio_service_start, audio_service_status);
}