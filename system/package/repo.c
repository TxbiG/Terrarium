#include "repo.h"

#include <stdio.h>
#include <string.h>

#include "store.h"
#include "../status.h"

int terra_repo_add(const terra_package_repo_t *repo) {
    if (!repo || !repo->name[0] || !repo->url[0])
        return TERRA_STATUS_INVAL;
    if (!repo->require_tls || !repo->require_signature)
        return TERRA_STATUS_ACCES;
    terra_package_store_t *store = terra_package_store_get();
    int existing = terra_package_store_find_repo(repo->name);
    if (existing >= 0) {
        store->repos[existing] = *repo;
        return TERRA_STATUS_OK;
    }
    if (store->repo_count >= TERRA_PACKAGE_STORE_MAX_REPOS)
        return TERRA_STATUS_NOSPC;
    store->repos[store->repo_count++] = *repo;
    return TERRA_STATUS_OK;
}

int terra_repo_remove(const char *name) {
    if (!name || !name[0])
        return TERRA_STATUS_INVAL;
    terra_package_store_t *store = terra_package_store_get();
    int index = terra_package_store_find_repo(name);
    if (index < 0)
        return TERRA_STATUS_NOENT;
    for (unsigned i = (unsigned)index + 1; i < store->repo_count; ++i)
        store->repos[i - 1] = store->repos[i];
    --store->repo_count;
    return TERRA_STATUS_OK;
}

int terra_repo_list(terra_package_repo_t *repos, unsigned long max_repos) {
    if (!repos && max_repos != 0)
        return TERRA_STATUS_INVAL;
    terra_package_store_t *store = terra_package_store_get();
    unsigned long count = store->repo_count < max_repos ? store->repo_count : max_repos;
    for (unsigned long i = 0; i < count; ++i)
        repos[i] = store->repos[i];
    return (int)store->repo_count;
}

int terra_repo_refresh_metadata(const char *name, terra_repo_metadata_t *out_metadata) {
    if (!name || !name[0] || !out_metadata)
        return TERRA_STATUS_INVAL;
    int index = terra_package_store_find_repo(name);
    if (index < 0)
        return TERRA_STATUS_NOENT;
    terra_package_store_t *store = terra_package_store_get();
    const terra_package_repo_t *repo = &store->repos[index];
    memset(out_metadata, 0, sizeof(*out_metadata));
    snprintf(out_metadata->repo_name, sizeof(out_metadata->repo_name), "%s", repo->name);
    snprintf(out_metadata->snapshot_id, sizeof(out_metadata->snapshot_id), "%s-snapshot-bootstrap", repo->channel[0] ? repo->channel : "stable");
    snprintf(out_metadata->metadata_hash, sizeof(out_metadata->metadata_hash), "metadata-%s-%u", repo->name, store->package_count);
    snprintf(out_metadata->signature_id, sizeof(out_metadata->signature_id), "%s", repo->trusted_root);
    out_metadata->generated_unix_ms = 0;
    out_metadata->package_count = store->package_count;
    return TERRA_STATUS_OK;
}

int terra_repo_verify_metadata(const terra_repo_metadata_t *metadata) {
    if (!metadata || !metadata->repo_name[0] || !metadata->snapshot_id[0] || !metadata->metadata_hash[0])
        return TERRA_STATUS_INVAL;
    if (!metadata->signature_id[0])
        return TERRA_STATUS_ACCES;
    int root = terra_package_store_find_root(metadata->signature_id);
    if (root < 0)
        return TERRA_STATUS_ACCES;
    terra_trust_root_t trust = terra_package_store_get()->roots[root];
    if (!trust.enabled || trust.revoked)
        return TERRA_STATUS_ACCES;
    return TERRA_STATUS_OK;
}
