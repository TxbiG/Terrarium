#ifndef TERRARIUM_SYSTEM_PACKAGE_UPDATE_H
#define TERRARIUM_SYSTEM_PACKAGE_UPDATE_H

#include <stdint.h>

#define TERRA_UPDATE_CHANNEL_MAX 32u
#define TERRA_UPDATE_VERSION_MAX 32u
#define TERRA_UPDATE_SNAPSHOT_MAX 64u

typedef enum terra_update_state {
    TERRA_UPDATE_IDLE = 0,
    TERRA_UPDATE_REFRESHING,
    TERRA_UPDATE_AVAILABLE,
    TERRA_UPDATE_DOWNLOADING,
    TERRA_UPDATE_VERIFYING,
    TERRA_UPDATE_READY,
    TERRA_UPDATE_APPLYING,
    TERRA_UPDATE_ROLLED_BACK,
    TERRA_UPDATE_FAILED
} terra_update_state_t;

typedef struct terra_update_status {
    terra_update_state_t state;
    char installed_version[TERRA_UPDATE_VERSION_MAX];
    char available_version[TERRA_UPDATE_VERSION_MAX];
    char channel[TERRA_UPDATE_CHANNEL_MAX];
    char snapshot_id[TERRA_UPDATE_SNAPSHOT_MAX];
    uint32_t package_count;
    uint64_t download_bytes;
    int last_error;
} terra_update_status_t;

int terra_package_refresh(void);
int terra_package_upgrade_all(void);
int terra_package_check_updates(void);
int terra_update_get_status(terra_update_status_t *out_status);
int terra_update_stage_snapshot(const char *snapshot_id);
int terra_update_apply_staged(void);
int terra_update_rollback(const char *snapshot_id);

#endif
