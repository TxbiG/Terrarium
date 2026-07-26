#include "Taskbar.h"

int terra_taskbar_height(void) {
    return 32;
}

int terra_taskbar_icon_size(void) {
    return 20;
}

void terra_taskbar_init(terra_taskbar_t *taskbar, terra_taskbar_edge_t edge, int screen_width, int screen_height) {
    if (!taskbar)
        return;

    int thickness = terra_taskbar_height();
    taskbar->edge = edge;
    taskbar->visible = 1;
    taskbar->blur_enabled = 0;
    taskbar->blur_strength = 0.0f;

    if (edge == TERRA_TASKBAR_LEFT || edge == TERRA_TASKBAR_RIGHT) {
        taskbar->rect.x = edge == TERRA_TASKBAR_LEFT ? 0 : screen_width - thickness;
        taskbar->rect.y = 0;
        taskbar->rect.width = thickness;
        taskbar->rect.height = screen_height;
    } else {
        taskbar->rect.x = 0;
        taskbar->rect.y = edge == TERRA_TASKBAR_TOP ? 0 : screen_height - thickness;
        taskbar->rect.width = screen_width;
        taskbar->rect.height = thickness;
    }
}

void terra_taskbar_set_visible(terra_taskbar_t *taskbar, int visible) {
    if (taskbar)
        taskbar->visible = visible ? 1 : 0;
}

void terra_taskbar_set_blur(terra_taskbar_t *taskbar, int enabled, float strength) {
    if (!taskbar)
        return;

    taskbar->blur_enabled = enabled ? 1 : 0;
    taskbar->blur_strength = strength;
}
