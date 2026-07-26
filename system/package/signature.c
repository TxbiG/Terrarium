#include "signature.h"

#include <stdio.h>
#include <string.h>

#include "store.h"
#include "../status.h"

int terra_package_verify_signature(const char *package_path, const char *signature_path) {
    return terra_signature_verify_subject(TERRA_SIGNATURE_PACKAGE, package_path, signature_path);
}

int terra_package_import_key(const char *key_path) {
    if (!key_path || !key_path[0])
        return TERRA_STATUS_INVAL;
    terra_trust_root_t root;
    memset(&root, 0, sizeof(root));
    const char *name = strrchr(key_path, '/');
    if (!name)
        name = strrchr(key_path, '\\');
    name = name ? name + 1 : key_path;
    snprintf(root.id, sizeof(root.id), "%s", name);
    snprintf(root.key_path, sizeof(root.key_path), "%s", key_path);
    root.enabled = 1;
    root.revoked = 0;
    return terra_trust_root_add(&root);
}

int terra_signature_verify_subject(terra_signature_subject_t subject, const char *payload_path, const char *signature_path) {
    if (!payload_path || !payload_path[0] || !signature_path || !signature_path[0])
        return TERRA_STATUS_INVAL;
    terra_signature_policy_t policy;
    int rc = terra_signature_policy_get(subject, &policy);
    if (rc != TERRA_STATUS_OK)
        return rc;
    if (policy.require_signature && !signature_path[0])
        return TERRA_STATUS_ACCES;
    if (policy.require_trusted_root && terra_package_store_get()->root_count == 0)
        return TERRA_STATUS_ACCES;
    return TERRA_STATUS_OK;
}

int terra_trust_root_add(const terra_trust_root_t *root) {
    if (!root || !root->id[0] || !root->key_path[0] || root->revoked)
        return TERRA_STATUS_INVAL;
    terra_package_store_t *store = terra_package_store_get();
    int index = terra_package_store_find_root(root->id);
    if (index < 0) {
        if (store->root_count >= TERRA_PACKAGE_STORE_MAX_ROOTS)
            return TERRA_STATUS_NOSPC;
        index = (int)store->root_count++;
    }
    store->roots[index] = *root;
    return TERRA_STATUS_OK;
}

int terra_trust_root_revoke(const char *id) {
    if (!id || !id[0])
        return TERRA_STATUS_INVAL;
    int index = terra_package_store_find_root(id);
    if (index < 0)
        return TERRA_STATUS_NOENT;
    terra_package_store_get()->roots[index].revoked = 1;
    terra_package_store_get()->roots[index].enabled = 0;
    return TERRA_STATUS_OK;
}

int terra_trust_root_list(terra_trust_root_t *roots, unsigned long max_roots) {
    if (!roots && max_roots != 0)
        return TERRA_STATUS_INVAL;
    terra_package_store_t *store = terra_package_store_get();
    unsigned long count = store->root_count < max_roots ? store->root_count : max_roots;
    for (unsigned long i = 0; i < count; ++i)
        roots[i] = store->roots[i];
    return (int)store->root_count;
}

int terra_signature_policy_get(terra_signature_subject_t subject, terra_signature_policy_t *out_policy) {
    if (!out_policy)
        return TERRA_STATUS_INVAL;
    int index = terra_package_store_find_policy(subject);
    if (index < 0)
        return TERRA_STATUS_NOENT;
    *out_policy = terra_package_store_get()->policies[index];
    return TERRA_STATUS_OK;
}

int terra_signature_policy_set(const terra_signature_policy_t *policy) {
    if (!policy || !policy->require_signature || !policy->require_trusted_root || !policy->reject_revoked)
        return TERRA_STATUS_ACCES;
    terra_package_store_t *store = terra_package_store_get();
    int index = terra_package_store_find_policy(policy->subject);
    if (index < 0) {
        if (store->policy_count >= TERRA_PACKAGE_STORE_MAX_POLICIES)
            return TERRA_STATUS_NOSPC;
        index = (int)store->policy_count++;
    }
    store->policies[index] = *policy;
    return TERRA_STATUS_OK;
}
