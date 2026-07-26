#include "xdg.h"
#include "wm.h"
#include "scene.h"
#include <stdio.h>
#include <string.h>

static lumina_view_t *find_view(lumina_compositor_t *compositor, unsigned view_id) {
    if (!compositor)
        return NULL;

    for (size_t i = 0; i < compositor->view_count; ++i) {
        if (compositor->views[i].id == view_id)
            return &compositor->views[i];
    }

    return NULL;
}

int lumina_xdg_add_view(lumina_compositor_t *compositor, const char *title) {
    lumina_view_t *view;

    if (!compositor || compositor->view_count >= LUMINA_MAX_VIEWS) {
        return -1;
    }

    view = &compositor->views[compositor->view_count];
    memset(view, 0, sizeof(*view));
    view->id = (unsigned)compositor->view_count + 1u;
    snprintf(view->title, sizeof(view->title), "%s", title ? title : "Untitled");
    view->mapped = 0;
    compositor->view_count++;
    lumina_compositor_emit(compositor, TERRA_DISPLAY_EVENT_VIEW_CREATED, view->id);
    lumina_xdg_configure_view(compositor, view->id, 640, 480);
    return 0;
}

unsigned lumina_xdg_configure_view(lumina_compositor_t *compositor, unsigned view_id, int width, int height) {
    lumina_view_t *view = find_view(compositor, view_id);
    unsigned serial;

    if (!view)
        return 0;

    if (width > 0)
        view->rect.width = width;
    if (height > 0)
        view->rect.height = height;

    serial = lumina_compositor_next_serial(compositor);
    view->pending_configure_serial = serial;
    lumina_compositor_emit_event(compositor, TERRA_DISPLAY_EVENT_VIEW_CONFIGURE, view_id, (unsigned)view->rect.width, (unsigned)view->rect.height, serial);
    return serial;
}

int lumina_xdg_ack_configure(lumina_compositor_t *compositor, unsigned view_id, unsigned serial) {
    lumina_view_t *view = find_view(compositor, view_id);

    if (!view || serial == 0 || view->pending_configure_serial != serial)
        return -1;

    view->acked_configure_serial = serial;
    view->pending_configure_serial = 0;
    view->mapped = 1;
    lumina_wm_arrange(compositor);
    lumina_scene_damage_view(compositor, view_id);
    if (compositor->focused_view < 0)
        lumina_wm_focus(compositor, view_id);
    return 0;
}

int lumina_xdg_request_close(lumina_compositor_t *compositor, unsigned view_id) {
    lumina_view_t *view = find_view(compositor, view_id);

    if (!view || !view->mapped)
        return -1;

    lumina_compositor_emit_event(compositor, TERRA_DISPLAY_EVENT_VIEW_CLOSE_REQUESTED, view_id, 0, 0, lumina_compositor_next_serial(compositor));
    return 0;
}

int lumina_xdg_close_view(lumina_compositor_t *compositor, unsigned view_id) {
    if (!compositor)
        return -1;

    for (size_t i = 0; i < compositor->view_count; ++i) {
        if (compositor->views[i].id != view_id || !compositor->views[i].mapped)
            continue;

        compositor->views[i].mapped = 0;
        compositor->views[i].focused = 0;
        compositor->views[i].pending_configure_serial = 0;
        if (compositor->focused_view == (int)i)
            compositor->focused_view = -1;
        lumina_compositor_emit(compositor, TERRA_DISPLAY_EVENT_VIEW_DESTROYED, view_id);
        lumina_scene_damage_view(compositor, view_id);
        lumina_wm_focus_next(compositor);
        lumina_wm_arrange(compositor);
        return 0;
    }

    return -1;
}

void lumina_xdg_init(lumina_compositor_t *compositor) {
    if (!compositor || compositor->view_count > 0) {
        return;
    }

    lumina_xdg_add_view(compositor, "Terrarium Shell");
}


