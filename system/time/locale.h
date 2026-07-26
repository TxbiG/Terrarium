#ifndef TERRARIUM_SYSTEM_TIME_LOCALE_H
#define TERRARIUM_SYSTEM_TIME_LOCALE_H

typedef struct terra_locale {
    char language[16];
    char region[16];
    char encoding[16];
} terra_locale_t;

int terra_locale_get(terra_locale_t *out_locale);
int terra_locale_set(const terra_locale_t *locale);

#endif
