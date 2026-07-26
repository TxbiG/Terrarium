#include <pwd.h>
#include <stdio.h>
#include <unistd.h>

int main(void) {
    struct passwd *pw = getpwuid(geteuid());

    if (pw && pw->pw_name) {
        puts(pw->pw_name);
        return 0;
    }

    printf("%lu\n", (unsigned long)geteuid());
    return 0;
}
