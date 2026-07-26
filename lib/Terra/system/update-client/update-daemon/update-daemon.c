#include <package/repo.h>
#include <package/signature.h>
#include <package/update.h>
#include <status.h>

#include <stdio.h>
#include <string.h>

static const char *update_state_name(terra_update_state_t state) {
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

static void print_status(const terra_update_status_t *status) {
    if (!status)
        return;
    printf("update-daemon: state=%s installed=%s available=%s channel=%s snapshot=%s packages=%u bytes=%llu last_error=%d\n",
           update_state_name(status->state),
           status->installed_version,
           status->available_version[0] ? status->available_version : "none",
           status->channel,
           status->snapshot_id[0] ? status->snapshot_id : "none",
           status->package_count,
           (unsigned long long)status->download_bytes,
           status->last_error);
}

static int configure_default_policy(void) {
    terra_signature_policy_t policy;
    int status = terra_signature_policy_get(TERRA_SIGNATURE_REPOSITORY, &policy);
    if (status != TERRA_STATUS_OK)
        return status;
    policy.require_signature = 1;
    policy.require_trusted_root = 1;
    policy.reject_revoked = 1;
    snprintf(policy.required_channel, sizeof(policy.required_channel), "stable");
    return terra_signature_policy_set(&policy);
}

static int refresh_repositories(void) {
    terra_package_repo_t repos[8];
    int status = terra_repo_list(repos, 8);
    if (status < 0)
        return status;

    for (int i = 0; i < status && i < 8; ++i) {
        if (!repos[i].enabled)
            continue;
        terra_repo_metadata_t metadata;
        int refresh = terra_repo_refresh_metadata(repos[i].name, &metadata);
        if (refresh < 0)
            return refresh;
        int verify = terra_repo_verify_metadata(&metadata);
        if (verify < 0)
            return verify;
    }

    return TERRA_STATUS_OK;
}

int main(int argc, char **argv) {
    int once = argc > 1 && strcmp(argv[1], "--once") == 0;
    int status = configure_default_policy();
    if (status < 0)
        fprintf(stderr, "update-daemon: signature policy backend unavailable: %d\n", status);

    status = refresh_repositories();
    if (status < 0)
        fprintf(stderr, "update-daemon: repository refresh backend unavailable: %d\n", status);

    status = terra_package_check_updates();
    if (status < 0)
        fprintf(stderr, "update-daemon: update check backend unavailable: %d\n", status);

    terra_update_status_t update_status;
    if (terra_update_get_status(&update_status) == TERRA_STATUS_OK)
        print_status(&update_status);

    if (!once)
        printf("update-daemon: persistent scheduling is pending the service timer/event-loop API\n");

    return status < 0 ? 2 : 0;
}
