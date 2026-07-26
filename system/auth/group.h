#ifndef TERRARIUM_SYSTEM_AUTH_GROUP_H
#define TERRARIUM_SYSTEM_AUTH_GROUP_H

#include "user.h"

typedef struct terra_group {
    terra_gid_t gid;
    char name[64];
} terra_group_t;

int terra_group_lookup(const char *name, terra_group_t *out_group);
int terra_group_lookup_gid(terra_gid_t gid, terra_group_t *out_group);
int terra_group_add_user(const char *group, const char *user);
int terra_group_remove_user(const char *group, const char *user);

#endif
