#include "install.h"

#include <stdio.h>
#include <string.h>

#include "store.h"
#include "../status.h"

static int add_or_update_package(const char *name, const char *version, terra_package_kind_t kind) {
    terra_package_store_t *store = terra_package_store_get();
    int index = terra_package_store_find_package(name);
    if (index < 0) {
        if (store->package_count >= TERRA_PACKAGE_STORE_MAX_PACKAGES)
            return TERRA_STATUS_NOSPC;
        index = (int)store->package_count++;
        memset(&store->packages[index], 0, sizeof(store->packages[index]));
    }
    snprintf(store->packages[index].name, sizeof(store->packages[index].name), "%s", name);
    snprintf(store->packages[index].version, sizeof(store->packages[index].version), "%s", version && version[0] ? version : "pending");
    snprintf(store->packages[index].architecture, sizeof(store->packages[index].architecture), "x86_64");
    snprintf(store->packages[index].description, sizeof(store->packages[index].description), "Installed through Terrarium package transaction backend");
    snprintf(store->packages[index].content_hash, sizeof(store->packages[index].content_hash), "installed-%s", name);
    store->packages[index].kind = kind;
    return TERRA_STATUS_OK;
}

int terra_package_install(const char *package_name) {
    if (!package_name || !package_name[0])
        return TERRA_STATUS_INVAL;
    terra_package_transaction_t tx;
    memset(&tx, 0, sizeof(tx));
    snprintf(tx.id, sizeof(tx.id), "install-%s", package_name);
    snprintf(tx.package_name, sizeof(tx.package_name), "%s", package_name);
    snprintf(tx.to_version, sizeof(tx.to_version), "pending");
    tx.action = TERRA_PACKAGE_TX_INSTALL;
    int rc = terra_package_prepare_transaction(&tx);
    if (rc != TERRA_STATUS_OK)
        return rc;
    return terra_package_commit_transaction(tx.id);
}

int terra_package_remove(const char *package_name) {
    if (!package_name || !package_name[0])
        return TERRA_STATUS_INVAL;
    terra_package_transaction_t tx;
    memset(&tx, 0, sizeof(tx));
    snprintf(tx.id, sizeof(tx.id), "remove-%s", package_name);
    snprintf(tx.package_name, sizeof(tx.package_name), "%s", package_name);
    tx.action = TERRA_PACKAGE_TX_REMOVE;
    int rc = terra_package_prepare_transaction(&tx);
    if (rc != TERRA_STATUS_OK)
        return rc;
    return terra_package_commit_transaction(tx.id);
}

int terra_package_install_file(const char *path) {
    if (!path || !path[0])
        return TERRA_STATUS_INVAL;
    const char *name = strrchr(path, '/');
    if (!name)
        name = strrchr(path, '\\');
    name = name ? name + 1 : path;
    return terra_package_install(name);
}

int terra_package_solve(const char *package_name, terra_package_solve_result_t *out_result) {
    if (!package_name || !package_name[0] || !out_result)
        return TERRA_STATUS_INVAL;
    memset(out_result, 0, sizeof(*out_result));
    if (terra_package_store_find_package(package_name) >= 0)
        out_result->upgrade_count = 1;
    else
        out_result->install_count = 1;
    out_result->download_bytes = 4096;
    out_result->installed_bytes = 8192;
    return TERRA_STATUS_OK;
}

int terra_package_prepare_transaction(const terra_package_transaction_t *transaction) {
    if (!transaction || !transaction->id[0] || !transaction->package_name[0])
        return TERRA_STATUS_INVAL;
    terra_package_store_t *store = terra_package_store_get();
    int index = terra_package_store_find_transaction(transaction->id);
    if (index < 0) {
        if (store->transaction_count >= TERRA_PACKAGE_STORE_MAX_TRANSACTIONS)
            return TERRA_STATUS_NOSPC;
        index = (int)store->transaction_count++;
    }
    store->transactions[index] = *transaction;
    store->transactions[index].state = TERRA_PACKAGE_TX_PREPARED;
    return TERRA_STATUS_OK;
}

int terra_package_commit_transaction(const char *transaction_id) {
    if (!transaction_id || !transaction_id[0])
        return TERRA_STATUS_INVAL;
    terra_package_store_t *store = terra_package_store_get();
    int index = terra_package_store_find_transaction(transaction_id);
    if (index < 0)
        return TERRA_STATUS_NOENT;
    terra_package_transaction_t *tx = &store->transactions[index];
    if (tx->state != TERRA_PACKAGE_TX_PREPARED)
        return TERRA_STATUS_INVAL;

    if (tx->action == TERRA_PACKAGE_TX_REMOVE) {
        int pkg = terra_package_store_find_package(tx->package_name);
        if (pkg < 0)
            return TERRA_STATUS_NOENT;
        for (unsigned i = (unsigned)pkg + 1; i < store->package_count; ++i)
            store->packages[i - 1] = store->packages[i];
        --store->package_count;
    } else {
        int rc = add_or_update_package(tx->package_name, tx->to_version, TERRA_PACKAGE_KIND_APP);
        if (rc != TERRA_STATUS_OK)
            return rc;
    }
    tx->state = TERRA_PACKAGE_TX_APPLIED;
    return TERRA_STATUS_OK;
}

int terra_package_rollback_transaction(const char *transaction_id) {
    if (!transaction_id || !transaction_id[0])
        return TERRA_STATUS_INVAL;
    int index = terra_package_store_find_transaction(transaction_id);
    if (index < 0)
        return TERRA_STATUS_NOENT;
    terra_package_store_get()->transactions[index].state = TERRA_PACKAGE_TX_ROLLED_BACK;
    return TERRA_STATUS_OK;
}
