#ifndef TERRARIUM_SYSTEM_RECOVERY_RECOVERY_H
#define TERRARIUM_SYSTEM_RECOVERY_RECOVERY_H

#include <stdint.h>

#define TERRA_RECOVERY_TARGET_MAX 64u
#define TERRA_RECOVERY_LOG_PATH_MAX 256u

typedef enum terra_recovery_mode {
    TERRA_RECOVERY_NORMAL = 0,
    TERRA_RECOVERY_SAFE_MODE,
    TERRA_RECOVERY_REPAIR_FS,
    TERRA_RECOVERY_ROLLBACK_PACKAGE,
    TERRA_RECOVERY_DISABLE_SERVICE,
    TERRA_RECOVERY_COLLECT_LOGS
} terra_recovery_mode_t;

typedef struct terra_recovery_plan {
    terra_recovery_mode_t mode;
    char target[TERRA_RECOVERY_TARGET_MAX];
    char log_path[TERRA_RECOVERY_LOG_PATH_MAX];
    uint32_t dry_run;
    uint32_t requires_reboot;
} terra_recovery_plan_t;

int terra_recovery_get_mode(terra_recovery_mode_t *out_mode);
int terra_recovery_enter_safe_mode(const char *reason);
int terra_recovery_collect_logs(const char *target_path);
int terra_recovery_repair_filesystem(const char *mount_path, uint32_t dry_run);
int terra_recovery_disable_service(const char *service_name);
int terra_recovery_rollback_package(const char *snapshot_id);
int terra_recovery_plan_apply(const terra_recovery_plan_t *plan);

#endif
