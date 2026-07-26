#ifndef TERRARIUM_UI_WIDGET_H
#define TERRARIUM_UI_WIDGET_H

#include "../../gfx.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum terra_widget_state {
    TERRA_WIDGET_VISIBLE = 1 << 0,
    TERRA_WIDGET_ENABLED = 1 << 1,
    TERRA_WIDGET_FOCUSED = 1 << 2,
} terra_widget_state_t;

typedef struct terra_widget {
    unsigned id;
    recti_t rect;
    const char *label;
    unsigned state;
    void *user_data;
} terra_widget_t;

terra_widget_t terra_widget_create(unsigned id, recti_t rect, const char *label);
void terra_widget_set_label(terra_widget_t *widget, const char *label);
void terra_widget_set_visible(terra_widget_t *widget, int visible);
void terra_widget_set_enabled(terra_widget_t *widget, int enabled);
int terra_widget_is_visible(const terra_widget_t *widget);
int terra_widget_is_enabled(const terra_widget_t *widget);

#ifdef __cplusplus
}
#endif

#endif
