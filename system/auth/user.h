#ifndef TERRARIUM_SYSTEM_AUTH_USER_H
#define TERRARIUM_SYSTEM_AUTH_USER_H

#include <stddef.h>
#include <stdint.h>

typedef uint32_t terra_uid_t;
typedef uint32_t terra_gid_t;

typedef struct terra_user {
    terra_uid_t uid;
    terra_gid_t primary_gid;
    char name[64];
    char home[256];
    char shell[256];
} terra_user_t;

int terra_user_lookup(const char *name, terra_user_t *out_user);
int terra_user_lookup_uid(terra_uid_t uid, terra_user_t *out_user);
int terra_user_create(const terra_user_t *user);
int terra_user_remove(const char *name);

#endif
