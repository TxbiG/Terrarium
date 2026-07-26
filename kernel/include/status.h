#ifndef TERRARIUM_STATUS_H
#define TERRARIUM_STATUS_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum terra_status {
    TERRA_STATUS_OK = 0,
    TERRA_STATUS_PERM = -1,
    TERRA_STATUS_NOENT = -2,
    TERRA_STATUS_IO = -5,
    TERRA_STATUS_BADF = -9,
    TERRA_STATUS_NOMEM = -12,
    TERRA_STATUS_ACCES = -13,
    TERRA_STATUS_BUSY = -16,
    TERRA_STATUS_EXIST = -17,
    TERRA_STATUS_NODEV = -19,
    TERRA_STATUS_NOTDIR = -20,
    TERRA_STATUS_ISDIR = -21,
    TERRA_STATUS_INVAL = -22,
    TERRA_STATUS_NOSPC = -28,
    TERRA_STATUS_ROFS = -30,
    TERRA_STATUS_NOSYS = -38,
    TERRA_STATUS_NOTSUP = -95,
    TERRA_STATUS_NOTREADY = -200,
} terra_status_t;

static inline int terra_status_is_error(int status) {
    return status < 0;
}

#ifdef __cplusplus
}
#endif

#endif