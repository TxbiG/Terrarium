#include "../common.h"

#include <grp.h>
#include <pwd.h>
#include <stdio.h>
#include <unistd.h>

int main(void) {
    uid_t uid = getuid();
    gid_t gid = getgid();
    struct passwd *pw = getpwuid(uid);
    struct group *gr = getgrgid(gid);

    printf("uid=%lu", (unsigned long)uid);
    if (pw && pw->pw_name)
        printf("(%s)", pw->pw_name);

    printf(" gid=%lu", (unsigned long)gid);
    if (gr && gr->gr_name)
        printf("(%s)", gr->gr_name);

    putchar('\n');
    return 0;
}
