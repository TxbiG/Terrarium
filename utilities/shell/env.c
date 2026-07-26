#include <stdio.h>

extern char **environ;

int main(void) {
    for (char **entry = environ; *entry; ++entry)
        puts(*entry);

    return 0;
}
