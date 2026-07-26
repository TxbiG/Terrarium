#ifndef TERRARIUM_NOVA_STATUSBAR_H
#define TERRARIUM_NOVA_STATUSBAR_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct nova_statusbar {
    char clock_text[32];
    char session_text[64];
    int battery_percent;
    int visible;
} nova_statusbar_t;

void nova_statusbar_init(nova_statusbar_t *bar);
void nova_statusbar_update(nova_statusbar_t *bar);
const char *nova_statusbar_summary(const nova_statusbar_t *bar, char *buffer, size_t buffer_size);

#ifdef __cplusplus
}
#endif

#endif
