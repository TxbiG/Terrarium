#include "../common.h"

#include <recovery/recovery.h>
#include <status.h>

static const char *mode_name(terra_recovery_mode_t mode) {
    switch (mode) {
    case TERRA_RECOVERY_NORMAL: return "normal";
    case TERRA_RECOVERY_SAFE_MODE: return "safe-mode";
    case TERRA_RECOVERY_REPAIR_FS: return "repair-fs";
    case TERRA_RECOVERY_ROLLBACK_PACKAGE: return "rollback-package";
    case TERRA_RECOVERY_DISABLE_SERVICE: return "disable-service";
    case TERRA_RECOVERY_COLLECT_LOGS: return "collect-logs";
    default: return "unknown";
    }
}

int main(int argc, char **argv) {
    if (util_is_help(argc, argv) || argc < 2) {
        util_usage("recoveryctl", "[status|safe-mode <reason>|collect-logs <path>|repair-fs <mount>|rollback <snapshot>|disable-service <name>]");
        return argc < 2 ? 1 : 0;
    }

    int rc = TERRA_STATUS_INVAL;
    if (strcmp(argv[1], "status") == 0) {
        terra_recovery_mode_t mode;
        rc = terra_recovery_get_mode(&mode);
        if (rc == TERRA_STATUS_OK) {
            printf("mode=%s\n", mode_name(mode));
            return 0;
        }
    } else if (strcmp(argv[1], "safe-mode") == 0 && argc > 2) {
        rc = terra_recovery_enter_safe_mode(argv[2]);
    } else if (strcmp(argv[1], "collect-logs") == 0 && argc > 2) {
        rc = terra_recovery_collect_logs(argv[2]);
    } else if (strcmp(argv[1], "repair-fs") == 0 && argc > 2) {
        rc = terra_recovery_repair_filesystem(argv[2], 0);
    } else if (strcmp(argv[1], "rollback") == 0 && argc > 2) {
        rc = terra_recovery_rollback_package(argv[2]);
    } else if (strcmp(argv[1], "disable-service") == 0 && argc > 2) {
        rc = terra_recovery_disable_service(argv[2]);
    }

    if (rc == TERRA_STATUS_INVAL) {
        util_usage("recoveryctl", "[status|safe-mode <reason>|collect-logs <path>|repair-fs <mount>|rollback <snapshot>|disable-service <name>]");
        return 1;
    }

    if (rc < 0)
        fprintf(stderr, "recoveryctl: backend unavailable (%d)\n", rc);
    return rc < 0 ? 2 : 0;
}
