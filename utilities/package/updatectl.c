#include "../common.h"

#include <package/update.h>
#include <status.h>

static const char *state_name(terra_update_state_t state) {
    switch (state) {
    case TERRA_UPDATE_IDLE: return "idle";
    case TERRA_UPDATE_REFRESHING: return "refreshing";
    case TERRA_UPDATE_AVAILABLE: return "available";
    case TERRA_UPDATE_DOWNLOADING: return "downloading";
    case TERRA_UPDATE_VERIFYING: return "verifying";
    case TERRA_UPDATE_READY: return "ready";
    case TERRA_UPDATE_APPLYING: return "applying";
    case TERRA_UPDATE_ROLLED_BACK: return "rolled-back";
    case TERRA_UPDATE_FAILED: return "failed";
    default: return "unknown";
    }
}

static int print_status(void) {
    terra_update_status_t status;
    int rc = terra_update_get_status(&status);
    if (rc < 0) {
        fprintf(stderr, "updatectl: status unavailable (%d)\n", rc);
        return 2;
    }
    printf("state=%s installed=%s available=%s channel=%s snapshot=%s packages=%u bytes=%llu last_error=%d\n",
           state_name(status.state), status.installed_version,
           status.available_version[0] ? status.available_version : "none",
           status.channel, status.snapshot_id[0] ? status.snapshot_id : "none",
           status.package_count, (unsigned long long)status.download_bytes, status.last_error);
    return status.last_error < 0 ? 2 : 0;
}

int main(int argc, char **argv) {
    if (util_is_help(argc, argv) || argc < 2) {
        util_usage("updatectl", "[status|refresh|check|stage <snapshot>|apply|rollback <snapshot>]");
        return argc < 2 ? 1 : 0;
    }

    int rc = TERRA_STATUS_INVAL;
    if (strcmp(argv[1], "status") == 0)
        return print_status();
    if (strcmp(argv[1], "refresh") == 0)
        rc = terra_package_refresh();
    else if (strcmp(argv[1], "check") == 0)
        rc = terra_package_check_updates();
    else if (strcmp(argv[1], "stage") == 0 && argc > 2)
        rc = terra_update_stage_snapshot(argv[2]);
    else if (strcmp(argv[1], "apply") == 0)
        rc = terra_update_apply_staged();
    else if (strcmp(argv[1], "rollback") == 0 && argc > 2)
        rc = terra_update_rollback(argv[2]);

    if (rc == TERRA_STATUS_INVAL) {
        util_usage("updatectl", "[status|refresh|check|stage <snapshot>|apply|rollback <snapshot>]");
        return 1;
    }

    if (rc < 0)
        fprintf(stderr, "updatectl: backend unavailable (%d)\n", rc);
    return rc < 0 ? 2 : 0;
}
