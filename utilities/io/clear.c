#include <stdio.h>

int main(void) {
    fputs("\033[2J\033[H", stdout);
    fflush(stdout);
    return 0;
}
