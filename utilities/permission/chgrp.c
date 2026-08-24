#include "../common.h"

int main(int argc, char **argv) {
    if (util_is_help(argc, argv)) { util_usage("chgrp", "GROUP FILE..."); return 0; }
    fprintf(stderr, "chgrp: Terrarium identity backend is not connected yet\n");
    return util_backend_missing("chgrp", "/etc/passwd", "group ownership API required");
}
