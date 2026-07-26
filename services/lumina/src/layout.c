#include "layout.h"

static lumina_rect_t lumina_primary_area(const lumina_compositor_t *compositor) {
    lumina_rect_t area = {0, 0, 1280, 720};

    if (compositor && compositor->output_count > 0) {
        area = compositor->outputs[0].rect;
    }

    return area;
}

void lumina_layout_apply(lumina_compositor_t *compositor, lumina_layout_t layout) {
    lumina_rect_t area;
    size_t mapped = 0;
    size_t stack_index = 0;
    size_t i;

    if (!compositor) {
        return;
    }

    compositor->layout = layout;
    area = lumina_primary_area(compositor);

    for (i = 0; i < compositor->view_count; ++i) {
        if (compositor->views[i].mapped && !compositor->views[i].floating) {
            mapped++;
        }
    }

    if (mapped == 0) {
        return;
    }

    for (i = 0; i < compositor->view_count; ++i) {
        lumina_view_t *view = &compositor->views[i];
        if (!view->mapped || view->floating) {
            continue;
        }

        if (layout == LUMINA_LAYOUT_MONOCLE || mapped == 1) {
            view->rect = area;
            continue;
        }

        if (layout == LUMINA_LAYOUT_FLOATING) {
            view->rect.x = area.x + 80 + (int)i * 24;
            view->rect.y = area.y + 60 + (int)i * 24;
            view->rect.width = area.width / 2;
            view->rect.height = area.height / 2;
            continue;
        }

        if (stack_index == 0) {
            view->rect.x = area.x;
            view->rect.y = area.y;
            view->rect.width = (area.width * 3) / 5;
            view->rect.height = area.height;
        } else {
            int stack_width = area.width - ((area.width * 3) / 5);
            int stack_height = area.height / (int)(mapped - 1);
            view->rect.x = area.x + ((area.width * 3) / 5);
            view->rect.y = area.y + (int)(stack_index - 1) * stack_height;
            view->rect.width = stack_width;
            view->rect.height = stack_height;
        }
        stack_index++;
    }
}

const char *lumina_layout_name(lumina_layout_t layout) {
    switch (layout) {
    case LUMINA_LAYOUT_TILE:
        return "tile";
    case LUMINA_LAYOUT_MONOCLE:
        return "monocle";
    case LUMINA_LAYOUT_FLOATING:
        return "floating";
    default:
        return "unknown";
    }
}
