#ifndef TERRARIUM_SYSTEM_AUTH_PASSWD_H
#define TERRARIUM_SYSTEM_AUTH_PASSWD_H

#include "user.h"

int terra_passwd_check(const char *user, const char *password);
int terra_passwd_set(const char *user, const char *password);
int terra_passwd_lock(const char *user);
int terra_passwd_unlock(const char *user);

#endif
