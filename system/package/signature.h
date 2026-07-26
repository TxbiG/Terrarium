#ifndef TERRARIUM_SYSTEM_PACKAGE_SIGNATURE_H
#define TERRARIUM_SYSTEM_PACKAGE_SIGNATURE_H

#include <stdint.h>

#define TERRA_SIGNATURE_ID_MAX 64u
#define TERRA_TRUST_ROOT_MAX 64u
#define TERRA_KEY_PATH_MAX 256u

typedef enum terra_signature_subject {
    TERRA_SIGNATURE_PACKAGE = 0,
    TERRA_SIGNATURE_REPOSITORY,
    TERRA_SIGNATURE_SERVICE,
    TERRA_SIGNATURE_DRIVER,
    TERRA_SIGNATURE_BOOT_IMAGE
} terra_signature_subject_t;

typedef enum terra_signature_decision {
    TERRA_SIGNATURE_DENY = 0,
    TERRA_SIGNATURE_ALLOW = 1
} terra_signature_decision_t;

typedef struct terra_trust_root {
    char id[TERRA_TRUST_ROOT_MAX];
    char key_path[TERRA_KEY_PATH_MAX];
    uint32_t enabled;
    uint32_t revoked;
} terra_trust_root_t;

typedef struct terra_signature_policy {
    terra_signature_subject_t subject;
    uint32_t require_signature;
    uint32_t require_trusted_root;
    uint32_t reject_revoked;
    char required_channel[32];
} terra_signature_policy_t;

int terra_package_verify_signature(const char *package_path, const char *signature_path);
int terra_package_import_key(const char *key_path);
int terra_signature_verify_subject(terra_signature_subject_t subject, const char *payload_path, const char *signature_path);
int terra_trust_root_add(const terra_trust_root_t *root);
int terra_trust_root_revoke(const char *id);
int terra_trust_root_list(terra_trust_root_t *roots, unsigned long max_roots);
int terra_signature_policy_get(terra_signature_subject_t subject, terra_signature_policy_t *out_policy);
int terra_signature_policy_set(const terra_signature_policy_t *policy);

#endif
