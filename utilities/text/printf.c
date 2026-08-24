#include "../common.h"

int main(int argc, char **argv) {
    if (argc < 2) { fputc('\n', stdout); return 0; }
    if (util_is_help(argc, argv)) { util_usage("printf", "FORMAT [ARGUMENT]..."); return 0; }

    /* Initial implementation delegates formatting to the C printf engine.
       Shell-specific escape expansion can be added when the Terrarium shell ABI lands. */
    printf("%s", argv[1]);
    for (int i = 2; i < argc; ++i) {
        fputc(' ', stdout);
        fputs(argv[i], stdout);
    }
    return 0;
}
