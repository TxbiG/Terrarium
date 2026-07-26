#include "recovery.h"

#include <stdio.h>
#include <string.h>

#include "../package/update.h"
#include "../runtime/service.h"
#include "../status.h"

#define TERRA_RECOVERY_DISABLED_SERVICE_MAX 32u

static terra_recovery_mode_t g_recovery_mode = TERRA_RECOVERY_NORMAL;
static char g_recovery_reason[TERRA_RECOVERY_TARGET_MAX];
static char g_last_log_path[TERRA_RECOVERY_LOG_PATH_MAX];
static char g_last_repaired_mount[TERRA_RECOVERY_TARGET_MAX];
static char g_disabled_services[TERRA_RECOVERY_DISABLED_SERVICE_MAX][TERRA_RECOVERY_TARGET_MAX];
static unsigned g_disabled_service_count;

static int remember_disabled_service(const char *service_name) {
    for (unsigned i = 0; i < g_disabled_service_count; ++i) {
        if (strcmp(g_disabled_services[i], service_name) == 0)
            return TERRA_STATUS_OK;
    }
    if (g_disabled_service_count >= TERRA_RECOVERY_DISABLED_SERVICE_MAX)
        return TERRA_STATUS_NOSPC;
    snprintf(g_disabled_services[g_disabled_service_count++], TERRA_RECOVERY_TARGET_MAX, "%s", service_name);
    return TERRA_STATUS_OK;
}

int terra_recovery_get_mode(terra_recovery_mode_t *out_mode) {
    if (!out_mode)
        return TERRA_STATUS_INVAL;
    *out_mode = g_recovery_mode;
    return TERRA_STATUS_OK;
}

int terra_recovery_enter_safe_mode(const char *reason) {
    if (!reason || !reason[0])
        return TERRA_STATUS_INVAL;
    snprintf(g_recovery_reason, sizeof(g_recovery_reason), "%s", reason);
    g_recovery_mode = TERRA_RECOVERY_SAFE_MODE;
    return TERRA_STATUS_OK;
}

int terra_recovery_collect_logs(const char *target_path) {
    if (!target_path || !target_path[0])
        return TERRA_STATUS_INVAL;
    snprintf(g_last_log_path, sizeof(g_last_log_path), "%s", target_path);
    g_recovery_mode = TERRA_RECOVERY_COLLECT_LOGS;
    return TERRA_STATUS_OK;
}

int terra_recovery_repair_filesystem(const char *mount_path, uint32_t dry_run) {
    if (!mount_path || !mount_path[0])
        return TERRA_STATUS_INVAL;
    snprintf(g_last_repaired_mount, sizeof(g_last_repaired_mount), "%s%s", mount_path, dry_run ? " (dry-run)" : "");
    g_recovery_mode = TERRA_RECOVERY_REPAIR_FS;
    return TERRA_STATUS_OK;
}

int terra_recovery_disable_service(const char *service_name) {
    if (!service_name || !service_name[0])
        return TERRA_STATUS_INVAL;
    int remembered = remember_disabled_service(service_name);
    if (remembered != TERRA_STATUS_OK)
        return remembered;
    int stopped = terra_service_stop(service_name);
    g_recovery_mode = TERRA_RECOVERY_DISABLE_SERVICE;
    return stopped == TERRA_STATUS_INVAL ? stopped : TERRA_STATUS_OK;
}

int terra_recovery_rollback_package(const char *snapshot_id) {
    int rc = terra_update_rollback(snapshot_id);
    if (rc == TERRA_STATUS_OK)
        g_recovery_mode = TERRA_RECOVERY_ROLLBACK_PACKAGE;
    return rc;
}

int terra_recovery_plan_apply(const terra_recovery_plan_t *plan) {
    if (!plan)
        return TERRA_STATUS_INVAL;
    switch (plan->mode) {
    case TERRA_RECOVERY_SAFE_MODE:
        return terra_recovery_enter_safe_mode(plan->target);
    case TERRA_RECOVERY_REPAIR_FS:
        return terra_recovery_repair_filesystem(plan->target, plan->dry_run);
    case TERRA_RECOVERY_ROLLBACK_PACKAGE:
        return terra_recovery_rollback_package(plan->target);
    case TERRA_RECOVERY_DISABLE_SERVICE:
        return terra_recovery_disable_service(plan->target);
    case TERRA_RECOVERY_COLLECT_LOGS:
        return terra_recovery_collect_logs(plan->log_path[0] ? plan->log_path : plan->target);
    case TERRA_RECOVERY_NORMAL:
        g_recovery_mode = TERRA_RECOVERY_NORMAL;
        return TERRA_STATUS_OK;
    default:
        return TERRA_STATUS_INVAL;
    }
}
