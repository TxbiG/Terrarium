



#include "../common.h"

#include <stdio.h>

int main(int argc, char *argv[]) {
    if (util_is_help(argc, argv) || argc < 2) {
        util_usage("objdump", "<object-file>");
        return argc < 2 ? 1 : 0;
    }

    return util_backend_missing("objdump", argv[1], "Terrarium object disassembler is not linked yet");
}
