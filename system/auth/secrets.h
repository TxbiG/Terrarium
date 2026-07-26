#ifndef TERRARIUM_SYSTEM_AUTH_SECRETS_H
#define TERRARIUM_SYSTEM_AUTH_SECRETS_H

#define TERRA_SECRET_NAME_MAX 64u
#define TERRA_SECRET_VALUE_MAX 256u

typedef struct terra_secret_record {
    char name[TERRA_SECRET_NAME_MAX];
    char value[TERRA_SECRET_VALUE_MAX];
} terra_secret_record_t;

int terra_secret_put(const char *name, const char *value);
int terra_secret_get(const char *name, char *out_value, unsigned long out_size);
int terra_secret_remove(const char *name);
int terra_secret_count(void);

#endif
