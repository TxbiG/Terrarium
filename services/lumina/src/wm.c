#include "wm.h"
#include "layout.h"

void lumina_wm_arrange(lumina_compositor_t *compositor) {
    if (!compositor) {
        return;
    }

    lumina_layout_apply(compositor, compositor->layout);
}

int lumina_wm_focus(lumina_compositor_t *compositor, unsigned view_id) {
    size_t i;

    if (!compositor) {
        return -1;
    }

    compositor->focused_view = -1;
    for (i = 0; i < compositor->view_count; ++i) {
        compositor->views[i].focused = compositor->views[i].mapped && compositor->views[i].id == view_id;
        if (compositor->views[i].focused) {
            compositor->focused_view = (int)i;
        }
    }

    if (compositor->focused_view >= 0) {
        lumina_compositor_emit(compositor, TERRA_DISPLAY_EVENT_VIEW_FOCUSED, view_id);
        return 0;
    }

    return -1;
}

int lumina_wm_focus_next(lumina_compositor_t *compositor) {
    if (!compositor || compositor->view_count == 0)
        return -1;

    size_t start = compositor->focused_view >= 0 ? (size_t)compositor->focused_view + 1u : 0u;
    for (size_t offset = 0; offset < compositor->view_count; ++offset) {
        size_t index = (start + offset) % compositor->view_count;
        if (compositor->views[index].mapped)
            return lumina_wm_focus(compositor, compositor->views[index].id);
    }

    compositor->focused_view = -1;
    return -1;
}