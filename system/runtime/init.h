#ifndef TERRARIUM_SYSTEM_RUNTIME_INIT_H
#define TERRARIUM_SYSTEM_RUNTIME_INIT_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum terra_boot_stage {
    TERRA_BOOT_STAGE_EARLY = 0,
    TERRA_BOOT_STAGE_SYSTEM,
    TERRA_BOOT_STAGE_SERVICES,
    TERRA_BOOT_STAGE_LOGIN,
    TERRA_BOOT_STAGE_RUNNING
} terra_boot_stage_t;

int terra_init_enter_stage(terra_boot_stage_t stage);
terra_boot_stage_t terra_init_current_stage(void);
const char *terra_init_stage_name(terra_boot_stage_t stage);

#ifdef __cplusplus
}
#endif

#endif