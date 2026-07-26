#include "init.h"

#include "../status.h"

static terra_boot_stage_t current_stage = TERRA_BOOT_STAGE_EARLY;

const char *terra_init_stage_name(terra_boot_stage_t stage) {
    switch (stage) {
    case TERRA_BOOT_STAGE_EARLY: return "early";
    case TERRA_BOOT_STAGE_SYSTEM: return "system";
    case TERRA_BOOT_STAGE_SERVICES: return "services";
    case TERRA_BOOT_STAGE_LOGIN: return "login";
    case TERRA_BOOT_STAGE_RUNNING: return "running";
    }
    return "unknown";
}

int terra_init_enter_stage(terra_boot_stage_t stage) {
    if (stage < TERRA_BOOT_STAGE_EARLY || stage > TERRA_BOOT_STAGE_RUNNING)
        return TERRA_STATUS_INVAL;
    if (stage < current_stage)
        return TERRA_STATUS_INVAL;

    current_stage = stage;
    return TERRA_STATUS_OK;
}

terra_boot_stage_t terra_init_current_stage(void) {
    return current_stage;
}