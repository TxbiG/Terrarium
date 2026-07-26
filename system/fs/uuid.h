#ifndef TERRARIUM_SYSTEM_FS_UUID_H
#define TERRARIUM_SYSTEM_FS_UUID_H

#include <stdint.h>

typedef struct terra_uuid {
    uint8_t bytes[16];
} terra_uuid_t;

int terra_uuid_generate(terra_uuid_t *out_uuid);
int terra_uuid_parse(const char *text, terra_uuid_t *out_uuid);
int terra_uuid_format(const terra_uuid_t *uuid, char *out_text, unsigned long out_size);

#endif
