#ifndef TERRARIUM_SYSTEM_PACKAGE_STORE_H
#define TERRARIUM_SYSTEM_PACKAGE_STORE_H

#include "install.h"
#include "package.h"
#include "repo.h"
#include "signature.h"

#define TERRA_PACKAGE_STORE_MAX_PACKAGES 64u
#define TERRA_PACKAGE_STORE_MAX_REPOS 16u
#define TERRA_PACKAGE_STORE_MAX_TRANSACTIONS 32u
#define TERRA_PACKAGE_STORE_MAX_ROOTS 16u
#define TERRA_PACKAGE_STORE_MAX_POLICIES 8u

typedef struct terra_package_store {
    terra_package_info_t packages[TERRA_PACKAGE_STORE_MAX_PACKAGES];
    unsigned package_count;
    terra_package_repo_t repos[TERRA_PACKAGE_STORE_MAX_REPOS];
    unsigned repo_count;
    terra_package_transaction_t transactions[TERRA_PACKAGE_STORE_MAX_TRANSACTIONS];
    unsigned transaction_count;
    terra_trust_root_t roots[TERRA_PACKAGE_STORE_MAX_ROOTS];
    unsigned root_count;
    terra_signature_policy_t policies[TERRA_PACKAGE_STORE_MAX_POLICIES];
    unsigned policy_count;
    unsigned initialized;
} terra_package_store_t;

terra_package_store_t *terra_package_store_get(void);
int terra_package_store_find_package(const char *name);
int terra_package_store_find_repo(const char *name);
int terra_package_store_find_transaction(const char *id);
int terra_package_store_find_root(const char *id);
int terra_package_store_find_policy(terra_signature_subject_t subject);

#endif
