#ifndef TERRARIUM_SYSTEM_TIME_TIMEZONE_H
#define TERRARIUM_SYSTEM_TIME_TIMEZONE_H

typedef struct terra_timezone {
    char name[64];
    int offset_minutes;
} terra_timezone_t;

int terra_timezone_get(terra_timezone_t *out_timezone);
int terra_timezone_set(const char *name);

#endif
