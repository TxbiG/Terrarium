#ifndef TERRARIUM_SYSTEM_AUTH_SHADOW_H
#define TERRARIUM_SYSTEM_AUTH_SHADOW_H

#include <stdint.h>

typedef struct terra_shadow_entry {
    char user[64];
    char password_hash[128];
    uint64_t last_change_epoch;
    uint32_t min_days;
    uint32_t max_days;
    uint32_t warn_days;
} terra_shadow_entry_t;

int terra_shadow_read(const char *user, terra_shadow_entry_t *out_entry);
int terra_shadow_write(const terra_shadow_entry_t *entry);

#endif
