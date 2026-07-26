#ifndef TERRARIUM_SYSTEM_PACKAGE_REPO_H
#define TERRARIUM_SYSTEM_PACKAGE_REPO_H

#include <stdint.h>

#define TERRA_REPO_NAME_MAX 64u
#define TERRA_REPO_URL_MAX 256u
#define TERRA_REPO_CHANNEL_MAX 32u
#define TERRA_REPO_ROOT_MAX 64u

typedef enum terra_repo_channel {
    TERRA_REPO_CHANNEL_STABLE = 0,
    TERRA_REPO_CHANNEL_BETA,
    TERRA_REPO_CHANNEL_DEV
} terra_repo_channel_t;

typedef struct terra_package_repo {
    char name[TERRA_REPO_NAME_MAX];
    char url[TERRA_REPO_URL_MAX];
    char channel[TERRA_REPO_CHANNEL_MAX];
    char trusted_root[TERRA_REPO_ROOT_MAX];
    int enabled;
    int require_tls;
    int require_signature;
} terra_package_repo_t;

typedef struct terra_repo_metadata {
    char repo_name[TERRA_REPO_NAME_MAX];
    char snapshot_id[64];
    char metadata_hash[65];
    char signature_id[64];
    uint64_t generated_unix_ms;
    uint32_t package_count;
} terra_repo_metadata_t;

int terra_repo_add(const terra_package_repo_t *repo);
int terra_repo_remove(const char *name);
int terra_repo_list(terra_package_repo_t *repos, unsigned long max_repos);
int terra_repo_refresh_metadata(const char *name, terra_repo_metadata_t *out_metadata);
int terra_repo_verify_metadata(const terra_repo_metadata_t *metadata);

#endif
