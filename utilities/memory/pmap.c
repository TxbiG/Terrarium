#include "../common.h"

int main(int argc, char **argv) {
    if (util_is_help(argc, argv) || argc < 2) {
        util_usage("pmap", "<pid>");
        return argc < 2 ? 1 : 0;
    }
    return util_backend_missing("pmap", "/proc/<pid>/maps", "process memory map export is not implemented yet");
}
