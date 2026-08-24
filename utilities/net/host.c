#include "../common.h"

#include <netdb.h>

int main(int argc, char **argv) {
    if (util_is_help(argc, argv)) { util_usage("host", "NAME"); return 0; }
    if (argc != 2) { util_usage("host", "NAME"); return 1; }

    struct addrinfo hints, *res = NULL;
    memset(&hints, 0, sizeof(hints));
    hints.ai_socktype = SOCK_STREAM;

    int rc = getaddrinfo(argv[1], NULL, &hints, &res);
    if (rc != 0) {
        fprintf(stderr, "host: %s\n", gai_strerror(rc));
        return 1;
    }

    printf("%s: address lookup succeeded\n", argv[1]);
    freeaddrinfo(res);
    return 0;
}
