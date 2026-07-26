#include "../common.h"

int main(int argc, char *argv[]) {
    if (util_is_help(argc, argv) || argc < 2) {
        util_usage("strace", "<command> [args...]");
        return argc < 2 ? 1 : 0;
    }

    return util_backend_missing("strace", "/proc/trace/syscalls", "syscall trace hooks are not exposed yet");
}
