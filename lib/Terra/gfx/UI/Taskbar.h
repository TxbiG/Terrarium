#ifndef TERRARIUM_UI_TASKBAR_H
#define TERRARIUM_UI_TASKBAR_H

#include "../gfx.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum terra_taskbar_edge {
    TERRA_TASKBAR_TOP = 0,
    TERRA_TASKBAR_BOTTOM,
    TERRA_TASKBAR_LEFT,
    TERRA_TASKBAR_RIGHT,
} terra_taskbar_edge_t;

typedef struct terra_taskbar {
    recti_t rect;
    terra_taskbar_edge_t edge;
    int visible;
    int blur_enabled;
    float blur_strength;
} terra_taskbar_t;

void terra_taskbar_init(terra_taskbar_t *taskbar, terra_taskbar_edge_t edge, int screen_width, int screen_height);
void terra_taskbar_set_visible(terra_taskbar_t *taskbar, int visible);
void terra_taskbar_set_blur(terra_taskbar_t *taskbar, int enabled, float strength);
int terra_taskbar_height(void);
int terra_taskbar_icon_size(void);

#ifdef __cplusplus
}
#endif

#endif
