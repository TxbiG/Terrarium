#include "../common.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[]) {
    if (util_is_help(argc, argv) || argc < 2) {
        util_usage("time", "<command> [args]...");
        return argc < 2 ? 1 : 0;
    }

    clock_t start = clock();
    int status = system(argv[1]);
    clock_t end = clock();

    double elapsed = (double)(end - start) / (double)CLOCKS_PER_SEC;
    fprintf(stderr, "elapsed %.3fs\n", elapsed);
    return status;
}
