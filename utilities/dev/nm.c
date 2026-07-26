#include "../common.h"

#include <stdio.h>

int main(int argc, char *argv[]) {
    if (util_is_help(argc, argv) || argc < 2) {
        util_usage("nm", "<object-file>");
        return argc < 2 ? 1 : 0;
    }

    return util_backend_missing("nm", argv[1], "Terrarium object symbol parser is not linked yet");
}
