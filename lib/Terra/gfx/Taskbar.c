#include "Taskbar.h"


void init_taskbar(Taskbar* taskbar, short position_type, int margin, int padding, Rect rect, Colour colour, Container* containers) {
    if (taskbar) {
        taskbar->position_type = position_type;
        taskbar->margin = margin;
        taskbar->padding = padding;
        taskbar->rect = rect;
        taskbar->colour = colour;
        taskbar->containers = containers;
    }
    if (containers) {
        containers->float_type = float_type;
        containers->margin = margin;
        containers->padding = padding;
        containers->rect = rect;
        containers->colour = colour;
    }
}


void terminate_taskbar(Taskbar* taskbar) {
    free(taskbar->containers);
    taskbar->m_containers = NULL;
}