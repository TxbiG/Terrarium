#include "../common.h"

int main(int argc, char **argv) {
    if (util_is_help(argc, argv)) { util_usage("tr", "SET1 SET2"); return 0; }
    if (argc != 3) { util_usage("tr", "SET1 SET2"); return 1; }

    const char *a = argv[1], *b = argv[2];
    int c;
    while ((c = getchar()) != EOF) {
        const char *p = strchr(a, c);
        if (p) {
            size_t index = (size_t)(p - a);
            if (index < strlen(b)) c = (unsigned char)b[index];
            else c = (unsigned char)b[strlen(b) - 1];
        }
        putchar(c);
    }
    return 0;
}
