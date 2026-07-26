#include "store.h"

#include <stdio.h>
#include <string.h>

static terra_package_store_t g_store;

static void init_policy(terra_signature_policy_t *policy, terra_signature_subject_t subject) {
    memset(policy, 0, sizeof(*policy));
    policy->subject = subject;
    policy->require_signature = 1;
    policy->require_trusted_root = 1;
    policy->reject_revoked = 1;
    snprintf(policy->required_channel, sizeof(policy->required_channel), "stable");
}

terra_package_store_t *terra_package_store_get(void) {
    if (!g_store.initialized) {
        memset(&g_store, 0, sizeof(g_store));
        snprintf(g_store.packages[0].name, sizeof(g_store.packages[0].name), "terrarium-base");
        snprintf(g_store.packages[0].version, sizeof(g_store.packages[0].version), "0.1.0");
        snprintf(g_store.packages[0].architecture, sizeof(g_store.packages[0].architecture), "x86_64");
        snprintf(g_store.packages[0].description, sizeof(g_store.packages[0].description), "TerrariumOS base system bootstrap package");
        g_store.packages[0].kind = TERRA_PACKAGE_KIND_SYSTEM;
        snprintf(g_store.packages[0].content_hash, sizeof(g_store.packages[0].content_hash), "bootstrap");
        g_store.package_count = 1;

        snprintf(g_store.repos[0].name, sizeof(g_store.repos[0].name), "stable");
        snprintf(g_store.repos[0].url, sizeof(g_store.repos[0].url), "https://updates.terrarium.local/stable");
        snprintf(g_store.repos[0].channel, sizeof(g_store.repos[0].channel), "stable");
        snprintf(g_store.repos[0].trusted_root, sizeof(g_store.repos[0].trusted_root), "terrarium-bootstrap-root");
        g_store.repos[0].enabled = 1;
        g_store.repos[0].require_tls = 1;
        g_store.repos[0].require_signature = 1;
        g_store.repo_count = 1;

        snprintf(g_store.roots[0].id, sizeof(g_store.roots[0].id), "terrarium-bootstrap-root");
        snprintf(g_store.roots[0].key_path, sizeof(g_store.roots[0].key_path), "/etc/terrarium/trust/bootstrap.pub");
        g_store.roots[0].enabled = 1;
        g_store.roots[0].revoked = 0;
        g_store.root_count = 1;

        init_policy(&g_store.policies[0], TERRA_SIGNATURE_PACKAGE);
        init_policy(&g_store.policies[1], TERRA_SIGNATURE_REPOSITORY);
        init_policy(&g_store.policies[2], TERRA_SIGNATURE_SERVICE);
        init_policy(&g_store.policies[3], TERRA_SIGNATURE_DRIVER);
        init_policy(&g_store.policies[4], TERRA_SIGNATURE_BOOT_IMAGE);
        g_store.policy_count = 5;
        g_store.initialized = 1;
    }
    return &g_store;
}

int terra_package_store_find_package(const char *name) {
    terra_package_store_t *store = terra_package_store_get();
    if (!name)
        return -1;
    for (unsigned i = 0; i < store->package_count; ++i) {
        if (strcmp(store->packages[i].name, name) == 0)
            return (int)i;
    }
    return -1;
}

int terra_package_store_find_repo(const char *name) {
    terra_package_store_t *store = terra_package_store_get();
    if (!name)
        return -1;
    for (unsigned i = 0; i < store->repo_count; ++i) {
        if (strcmp(store->repos[i].name, name) == 0)
            return (int)i;
    }
    return -1;
}

int terra_package_store_find_transaction(const char *id) {
    terra_package_store_t *store = terra_package_store_get();
    if (!id)
        return -1;
    for (unsigned i = 0; i < store->transaction_count; ++i) {
        if (strcmp(store->transactions[i].id, id) == 0)
            return (int)i;
    }
    return -1;
}

int terra_package_store_find_root(const char *id) {
    terra_package_store_t *store = terra_package_store_get();
    if (!id)
        return -1;
    for (unsigned i = 0; i < store->root_count; ++i) {
        if (strcmp(store->roots[i].id, id) == 0)
            return (int)i;
    }
    return -1;
}

int terra_package_store_find_policy(terra_signature_subject_t subject) {
    terra_package_store_t *store = terra_package_store_get();
    for (unsigned i = 0; i < store->policy_count; ++i) {
        if (store->policies[i].subject == subject)
            return (int)i;
    }
    return -1;
}
