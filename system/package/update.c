#include "update.h"

#include <stdio.h>
#include <string.h>

#include "install.h"
#include "repo.h"
#include "store.h"
#include "../status.h"

static terra_update_status_t g_update_status = {
    .state = TERRA_UPDATE_IDLE,
    .installed_version = "0.1.0",
    .available_version = "",
    .channel = "stable",
    .snapshot_id = "",
    .package_count = 0,
    .download_bytes = 0,
    .last_error = TERRA_STATUS_OK,
};

static int set_update_status(terra_update_state_t state, int status) {
    g_update_status.state = status < 0 ? TERRA_UPDATE_FAILED : state;
    g_update_status.last_error = status;
    return status;
}

int terra_package_refresh(void) {
    terra_package_store_t *store = terra_package_store_get();
    g_update_status.state = TERRA_UPDATE_REFRESHING;
    g_update_status.package_count = store->package_count;
    g_update_status.download_bytes = (uint64_t)store->package_count * 4096ull;
    for (unsigned i = 0; i < store->repo_count; ++i) {
        if (!store->repos[i].enabled)
            continue;
        terra_repo_metadata_t metadata;
        int rc = terra_repo_refresh_metadata(store->repos[i].name, &metadata);
        if (rc != TERRA_STATUS_OK)
            return set_update_status(TERRA_UPDATE_FAILED, rc);
        rc = terra_repo_verify_metadata(&metadata);
        if (rc != TERRA_STATUS_OK)
            return set_update_status(TERRA_UPDATE_FAILED, rc);
        snprintf(g_update_status.snapshot_id, sizeof(g_update_status.snapshot_id), "%s", metadata.snapshot_id);
    }
    return set_update_status(TERRA_UPDATE_IDLE, TERRA_STATUS_OK);
}

int terra_package_upgrade_all(void) {
    if (!g_update_status.snapshot_id[0])
        return set_update_status(TERRA_UPDATE_FAILED, TERRA_STATUS_INVAL);
    terra_package_transaction_t tx;
    memset(&tx, 0, sizeof(tx));
    snprintf(tx.id, sizeof(tx.id), "upgrade-%s", g_update_status.snapshot_id);
    snprintf(tx.package_name, sizeof(tx.package_name), "terrarium-base");
    snprintf(tx.from_version, sizeof(tx.from_version), "%s", g_update_status.installed_version);
    snprintf(tx.to_version, sizeof(tx.to_version), "%s", g_update_status.available_version[0] ? g_update_status.available_version : g_update_status.installed_version);
    tx.action = TERRA_PACKAGE_TX_UPGRADE;
    int rc = terra_package_prepare_transaction(&tx);
    if (rc != TERRA_STATUS_OK)
        return set_update_status(TERRA_UPDATE_FAILED, rc);
    rc = terra_package_commit_transaction(tx.id);
    if (rc != TERRA_STATUS_OK)
        return set_update_status(TERRA_UPDATE_FAILED, rc);
    snprintf(g_update_status.installed_version, sizeof(g_update_status.installed_version), "%s", tx.to_version);
    return set_update_status(TERRA_UPDATE_READY, TERRA_STATUS_OK);
}

int terra_package_check_updates(void) {
    int rc = terra_package_refresh();
    if (rc != TERRA_STATUS_OK)
        return rc;
    snprintf(g_update_status.available_version, sizeof(g_update_status.available_version), "%s", g_update_status.installed_version);
    return set_update_status(TERRA_UPDATE_AVAILABLE, TERRA_STATUS_OK);
}

int terra_update_get_status(terra_update_status_t *out_status) {
    if (!out_status)
        return TERRA_STATUS_INVAL;
    *out_status = g_update_status;
    return TERRA_STATUS_OK;
}

int terra_update_stage_snapshot(const char *snapshot_id) {
    if (!snapshot_id || !snapshot_id[0])
        return set_update_status(TERRA_UPDATE_FAILED, TERRA_STATUS_INVAL);
    snprintf(g_update_status.snapshot_id, sizeof(g_update_status.snapshot_id), "%s", snapshot_id);
    return set_update_status(TERRA_UPDATE_READY, TERRA_STATUS_OK);
}

int terra_update_apply_staged(void) {
    if (!g_update_status.snapshot_id[0])
        return set_update_status(TERRA_UPDATE_FAILED, TERRA_STATUS_INVAL);
    g_update_status.state = TERRA_UPDATE_APPLYING;
    return terra_package_upgrade_all();
}

int terra_update_rollback(const char *snapshot_id) {
    if (!snapshot_id || !snapshot_id[0])
        return set_update_status(TERRA_UPDATE_FAILED, TERRA_STATUS_INVAL);
    snprintf(g_update_status.snapshot_id, sizeof(g_update_status.snapshot_id), "%s", snapshot_id);
    return set_update_status(TERRA_UPDATE_ROLLED_BACK, TERRA_STATUS_OK);
}
