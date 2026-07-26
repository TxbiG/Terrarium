#include <stdio.h>
#include "session.h"
#include "auth.h"

int main(void) {
#if defined(TERRARIUM_DESKTOP) || defined(TERRARIUM_MOBILE) || defined(TERRARIUM_TABLET) || defined(TERRARIUM_RASPBERRY_PI)
    return 0;
#else
    char username[LOGIN_NAME_MAX];
    char password[LOGIN_NAME_MAX];
    login_account_t account;

    printf("Login: ");
    if (scanf("%63s", username) != 1)
        return 1;

    printf("Password: ");
    if (scanf("%63s", password) != 1)
        return 1;

    if (!login_authenticate(username, password, &account)) {
        printf("Authentication failed\n");
        return 1;
    }

    printf("Welcome, %s!\n", account.name);
    return login_session_start(&account) == 0 ? 0 : 1;
#endif
}