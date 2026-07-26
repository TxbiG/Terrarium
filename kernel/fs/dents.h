#ifndef TERRARIUM_FS_DENTS_H
#define TERRARIUM_FS_DENTS_H

#include "terra_abi.h"

typedef enum DENTS_RES {
    DENTS_OK = 0,
    DENTS_RETURN = 1,
    DENTS_NO_SPACE = 2,
} DENTS_RES;

static inline size_t terra_dent_strlen(const char *text, size_t max) {
    size_t len = 0;
    if (!text)
        return 0;
    while (len < max && text[len])
        ++len;
    return len;
}

static inline void terra_dent_copy(char *dst, const void *src, size_t len) {
    const char *in = (const char *)src;
    size_t i;
    for (i = 0; i < len; ++i)
        dst[i] = in[i];
    dst[len] = '\0';
}

static inline DENTS_RES dentsAdd(void *start, terra_dirent_t **cursor, size_t *allocated,
                                 unsigned int hardlimit, const void *name, size_t name_len,
                                 uint64 inode, uint8 type) {
    terra_dirent_t *entry;
    size_t reclen;

    if (!start || !cursor || !allocated || !name)
        return DENTS_NO_SPACE;
    if (name_len == 0)
        name_len = terra_dent_strlen((const char *)name, TERRA_DIRENT_NAME_MAX - 1);
    if (name_len >= TERRA_DIRENT_NAME_MAX)
        name_len = TERRA_DIRENT_NAME_MAX - 1;

    reclen = sizeof(terra_dirent_t);
    if (*allocated + reclen > hardlimit)
        return *allocated == 0 ? DENTS_NO_SPACE : DENTS_RETURN;

    entry = *cursor;
    entry->d_ino = inode;
    entry->d_off = *allocated + reclen;
    entry->d_reclen = (uint16)reclen;
    entry->d_type = type;
    terra_dent_copy(entry->d_name, name, name_len);

    *allocated += reclen;
    *cursor = (terra_dirent_t *)((uint8 *)start + *allocated);
    return DENTS_OK;
}

#endif
