#ifndef TERRARIUM_SYSTEM_FS_FSCK_H
#define TERRARIUM_SYSTEM_FS_FSCK_H

typedef enum terra_fsck_result {
    TERRA_FSCK_CLEAN = 0,
    TERRA_FSCK_REPAIRED,
    TERRA_FSCK_ERRORS,
    TERRA_FSCK_UNSUPPORTED
} terra_fsck_result_t;

terra_fsck_result_t terra_fsck(const char *device, const char *type, int repair);

#endif
