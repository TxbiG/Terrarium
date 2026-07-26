#ifndef LOGIN_AUTH_H
#define LOGIN_AUTH_H

#include <stdint.h>

#define LOGIN_NAME_MAX 64u
#define LOGIN_PATH_MAX 256u

typedef struct login_account {
    char name[LOGIN_NAME_MAX];
    uint32_t uid;
    uint32_t gid;
    char home[LOGIN_PATH_MAX];
    char shell[LOGIN_PATH_MAX];
    char password[LOGIN_NAME_MAX];
} login_account_t;

int login_lookup_account(const char *user, login_account_t *out_account);
int login_authenticate(const char *user, const char *password, login_account_t *out_account);

#endif // LOGIN_AUTH_H