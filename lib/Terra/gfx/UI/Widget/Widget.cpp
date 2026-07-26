#include "Widget.h"

terra_widget_t terra_widget_create(unsigned id, recti_t rect, const char *label) {
    terra_widget_t widget;
    widget.id = id;
    widget.rect = rect;
    widget.label = label;
    widget.state = TERRA_WIDGET_VISIBLE | TERRA_WIDGET_ENABLED;
    widget.user_data = 0;
    return widget;
}

void terra_widget_set_label(terra_widget_t *widget, const char *label) {
    if (widget)
        widget->label = label;
}

void terra_widget_set_visible(terra_widget_t *widget, int visible) {
    if (!widget)
        return;

    if (visible)
        widget->state |= TERRA_WIDGET_VISIBLE;
    else
        widget->state &= ~TERRA_WIDGET_VISIBLE;
}

void terra_widget_set_enabled(terra_widget_t *widget, int enabled) {
    if (!widget)
        return;

    if (enabled)
        widget->state |= TERRA_WIDGET_ENABLED;
    else
        widget->state &= ~TERRA_WIDGET_ENABLED;
}

int terra_widget_is_visible(const terra_widget_t *widget) {
    return widget ? ((widget->state & TERRA_WIDGET_VISIBLE) != 0) : 0;
}

int terra_widget_is_enabled(const terra_widget_t *widget) {
    return widget ? ((widget->state & TERRA_WIDGET_ENABLED) != 0) : 0;
}
